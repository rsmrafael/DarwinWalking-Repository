/*
 * HTTPServer.cpp
 *
 *  Created on: 19.01.2013
 *      Author: Stefan Krupop
 */

#include "HTTPServer.h"
#include "../Debugging/Debugger.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef WIN32
#include <signal.h>
#endif

#define BUFFER_SIZE		1024
#define TEN_K			(10 * 1024)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))	//lint -e(767) defined in other module
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) //lint -e(767) defined in other module
#define LENGTH_OF(x) (sizeof(x)/sizeof(x[0]))

/*
 * Standard header to be send along with other header information like mimetype.
 *
 * The parameters should ensure the browser does not cache our answer.
 * A browser should connect for each file and not serve files from his cache.
 * Using cached pictures would lead to showing old/outdated pictures
 * Many browser seem to ignore, or at least not always obey those headers
 * since i observed caching of files from time to time.
 */
#define STD_HEADER "Connection: close\r\n" \
                   "Server: MJPG-Streamer/0.2\r\n" \
                   "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
                   "Pragma: no-cache\r\n" \
                   "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n"

// the boundary is used for the M-JPEG stream, it separates the multipart stream of pictures
#define BOUNDARY "boundarydonotcross"

const HTTPServer::HTTPServerClient::mimetype_s HTTPServer::HTTPServerClient::mimetypes[] = { { ".html", "text/html" }, { ".htm", "text/html" }, { ".css", "text/css" }, { ".js", "text/javascript" }, { ".txt", "text/plain" }, { ".jpg", "image/jpeg" }, { ".jpeg", "image/jpeg" }, { ".png", "image/png" }, { ".gif", "image/gif" }, { ".ico", "image/x-icon" }, { ".swf", "application/x-shockwave-flash" }, { ".cab", "application/x-shockwave-flash" }, { ".jar", "application/java-archive" } };

HTTPServer::HTTPServer(uint16_t port, size_t dataBufferSize, const char* wwwFolder, const char* credentials) : mPort(port), mWWWFolder(wwwFolder), mCredentials(credentials), mClientRequest(false) {
    pthread_mutex_init(&Mutex, NULL);
    pthread_cond_init(&mCondBufferUpdated, NULL);
    mDataBuffer = (uint8_t*)malloc(dataBufferSize);
    mDataBufferSize = 0;
    mSD = 0;
#ifndef WIN32
    // We expect write failures to occur but we want to handle them where
    // the error occurs rather than in a SIGPIPE handler.
    signal(SIGPIPE, SIG_IGN);
#endif
	this->start(this);
}

HTTPServer::~HTTPServer() {
	Debugger::DEBUG("HTTPServer", "Stopping server thread...");
	shutdown(mSD, 2);
#ifdef WIN32
	closesocket(mSD);
#else
   	close(mSD);
#endif
	this->stop();
	Debugger::DEBUG("HTTPServer", "Server halted");
    pthread_cond_destroy(&mCondBufferUpdated);
    pthread_mutex_destroy(&Mutex);
    free(mDataBuffer);
    mWWWFolder = NULL;
    mCredentials = NULL;
}

bool HTTPServer::getClientRequest() const {
	return mClientRequest;
}

void HTTPServer::execute(void* arg) {
	struct sockaddr_in addr, client_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	// open socket for server
	//lint -e{641} Enum to int
	mSD = socket(PF_INET, SOCK_STREAM, 0);
	if (mSD == INVALID_SOCKET) {
		Debugger::ERR("HTTPServer", "Could not create server socket!");
		return;
	}

	// ignore "socket already in use" errors
	int on = 1;
	if (setsockopt(mSD, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, (socklen_t)sizeof(on)) < 0) {
		Debugger::ERR("HTTPServer", "setsockopt(SO_REUSEADDR) failed!");
		return;
	}

	// perhaps we will use this keep-alive feature oneday
	// setsockopt(mSD, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));

	// configure server address to listen to all local IPs
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(mPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(mSD, (struct sockaddr*) &addr, (socklen_t)sizeof(addr)) != 0) {
		Debugger::ERR("HTTPServer", "Could not bind server socket!");
		return;
	}

	// start listening on socket
	if (listen(mSD, 10) != 0) {
		Debugger::ERR("HTTPServer", "Could not listen on server socket!");
		return;
	}

	// create a child for every client that connects
	while (IsRunning()) {
		Debugger::DEBUG("HTTPServer", "Waiting for clients to connect");
		SOCKET clientSocket = accept(mSD, (struct sockaddr *) &client_addr, &addr_len);
		if (clientSocket != INVALID_SOCKET) {
			new HTTPServerClient(this, clientSocket);
		}
	}

#ifdef WIN32
	closesocket(mSD);
#else
   	close(mSD);
#endif
   	Debugger::DEBUG("HTTPServer", "Server thread ending");
}

const char* HTTPServer::getCredentials() const {
	return mCredentials;
}

const char* HTTPServer::getWWWFolder() const {
	return mWWWFolder;
}

void HTTPServer::setClientRequest(bool state) {
	mClientRequest = state;
}

void HTTPServer::waitForDataBufferUpdate() {
	pthread_cond_wait(&mCondBufferUpdated, &Mutex);
}

size_t HTTPServer::getDataBufferSize() const {
	return mDataBufferSize;
}

void HTTPServer::setDataBufferSize(size_t size) {
	mDataBufferSize = size;
}

uint8_t* HTTPServer::getDataBuffer() {
	return mDataBuffer;
}

void HTTPServer::notifyDataBufferUpdate() {
	pthread_cond_broadcast(&mCondBufferUpdated);
}

HTTPServer::HTTPServerClient::HTTPServerClient(HTTPServer* server, SOCKET clientSocket) : mServer(server), mSocket(clientSocket) {
	mRequestType = A_UNKNOWN;
	mRequestCredentials = NULL;
	mRequestParameter = NULL;
	mRequestClient = NULL;
	this->start(this);
}

HTTPServer::HTTPServerClient::~HTTPServerClient() {
	this->stop();
	mServer = NULL;
	free(mRequestParameter);
	free(mRequestClient);
	free(mRequestCredentials);
}

void HTTPServer::HTTPServerClient::execute(void* arg) {
	int cnt;
	char buffer[BUFFER_SIZE] = { 0 };
	char* pb = &buffer[0];
	iobuffer iobuf;

	// initializes the structures
	memset(iobuf.buffer, 0, sizeof(iobuf.buffer));
	iobuf.level = 0;
	mRequestType = A_UNKNOWN;
	mRequestParameter = NULL;
	mRequestClient = NULL;
	mRequestCredentials = NULL;

	// What does the client want to receive? Read the request.
	memset(buffer, 0, sizeof(buffer));
	size_t len = sizeof(buffer) - 1;
	if ((cnt = _readline(&iobuf, buffer, len, 5)) == -1) {
#ifdef WIN32
		closesocket(mSocket);
#else
		close(mSocket);
#endif
		return;
	}

	// determine what to deliver
	if (strstr(buffer, "GET /?action=snapshot") != NULL) {
		mRequestType = A_SNAPSHOT;
	} else if (strstr(buffer, "GET /?action=stream") != NULL) {
		mRequestType = A_STREAM;
	} else if (strstr(buffer, "GET /?action=command") != NULL) {
		size_t len;
		mRequestType = A_COMMAND;

		/* advance by the length of known string */
		if ((pb = strstr(buffer, "GET /?action=command")) == NULL) {
			Debugger::ERR("HTTPServer", "HTTP request seems to be malformed");
			send_error(400, "Malformed HTTP request");
#ifdef WIN32
			closesocket(mSocket);
#else
			close(mSocket);
#endif
			return;
		}
		pb += strlen("GET /?action=command");

		/* only accept certain characters */
		size_t matchlen = strspn(pb, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-=&1234567890");
		len = MIN(MAX(matchlen, 0), 100);
		mRequestParameter = (char*)malloc(len + 1);
		if (mRequestParameter == NULL) {
			Debugger::ERR("HTTPServer", "Could not allocate memory");
			return;
		}
		memset(mRequestParameter, 0, len + 1);
		strncpy(mRequestParameter, pb, len);

		Debugger::DEBUG("HTTPServer", "command parameter (len: %d): \"%s\"", len, mRequestParameter);
	} else {
		size_t len;

		Debugger::DEBUG("HTTPServer", "try to serve a file");
		mRequestType = A_FILE;

		if ((pb = strstr(buffer, "GET /")) == NULL) {
			Debugger::ERR("HTTPServer", "HTTP request seems to be malformed");
			send_error(400, "Malformed HTTP request");
#ifdef WIN32
			closesocket(mSocket);
#else
			close(mSocket);
#endif
			return;
		}

		pb += strlen("GET /");
		size_t matchlen = strspn(pb, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._-1234567890");
		len = MIN(MAX(matchlen, 0), 100);
		mRequestParameter = (char*)malloc(len + 1);
		if (mRequestParameter == NULL) {
			Debugger::ERR("HTTPServer", "Could not allocate memory");
			return;
		}
		memset(mRequestParameter, 0, len + 1);
		strncpy(mRequestParameter, pb, len);

		Debugger::DEBUG("HTTPServer", "parameter (len: %d): \"%s\"", len, mRequestParameter);
	}

	/*
	 * parse the rest of the HTTP-request
	 * the end of the request-header is marked by a single, empty line with "\r\n"
	 */
	do {
		memset(buffer, 0, sizeof(buffer));

		size_t len = sizeof(buffer) - 1;
		if ((cnt = _readline(&iobuf, buffer, len, 5)) == -1) {
			free_request();
#ifdef WIN32
			closesocket(mSocket);
#else
			close(mSocket);
#endif
			return;
		}

		if (strstr(buffer, "User-Agent: ") != NULL) {
			mRequestClient = strdup(buffer + strlen("User-Agent: "));
		} else if (strstr(buffer, "Authorization: Basic ") != NULL) {
			mRequestCredentials = strdup(buffer + strlen("Authorization: Basic "));
			decodeBase64(mRequestCredentials);
			Debugger::DEBUG("HTTPServer", "username:password: %s", mRequestCredentials);
		}

	} while (cnt > 2 && !(buffer[0] == '\r' && buffer[1] == '\n'));

	/* check for username and password if parameter -c was given */
	if (mServer->getCredentials() != NULL) {
		if (mRequestCredentials == NULL || strcmp(mServer->getCredentials(), mRequestCredentials) != 0) {
			Debugger::WARN("HTTPServer", "Access denied");
			send_error(401, "Username and password do not match to configuration");
#ifdef WIN32
			closesocket(mSocket);
#else
			close(mSocket);
#endif
			free_request();
			return;
		}
		Debugger::DEBUG("HTTPServer", "Access granted");
	}

	mServer->setClientRequest(true);

	/* now it's time to answer */
	switch (mRequestType) {
		case A_SNAPSHOT:
			Debugger::DEBUG("HTTPServer", "Request for snapshot");
			send_snapshot();
			break;
		case A_STREAM:
			Debugger::DEBUG("HTTPServer", "Request for stream");
			send_stream();
			break;
		case A_COMMAND:
			send_error(501, "this server is configured to not accept commands");
			break;
		case A_FILE:
			if (mServer->getWWWFolder() == NULL) {
				send_error(501, "no www-folder configured");
			} else {
				send_file(mRequestParameter);
			}
			break;
		case A_UNKNOWN:
		default:
			Debugger::ERR("HTTPServer", "unknown request");
			break;
	}

#ifdef WIN32
	closesocket(mSocket);
#else
   	close(mSocket);
#endif
	free_request();

	Debugger::DEBUG("HTTPServer", "leaving HTTP client thread");
   	delete this;
}

int HTTPServer::HTTPServerClient::_read(iobuffer* iobuf, void* buffer, size_t len, int timeout) {
	int rc;
	size_t i;
	size_t copied = 0;
	fd_set fds;
	struct timeval tv;

	memset(buffer, 0, len);

	while ((copied < len)) {
		i = MIN(iobuf->level, len-copied);
		memcpy(((char*) buffer) + copied, iobuf->buffer + IO_BUFFER_SIZE - iobuf->level, i);

		iobuf->level -= i;
		copied += i;
		if (copied >= len)
			return (int)copied;

		// select will return in case of timeout or new data arrived
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(mSocket, &fds);
		if ((rc = select((int)(mSocket + 1), &fds, NULL, NULL, &tv)) <= 0) {
			if (rc < 0) {
				Debugger::ERR("HTTPServer", "Select returned error");
				return 0;
			}

			// this must be a timeout
			return (int)copied;
		}

		memset(iobuf->buffer, 0, sizeof(iobuf->buffer));
		iobuf->level = 0;

		/*
		 * there should be at least one byte, because select signaled it.
		 * But: It may happen (very seldom), that the socket gets closed remotely between
		 * the select() and the following read. That is the reason for not relying
		 * on reading at least one byte.
		 */
		ssize_t r;
		if ((r = recv(mSocket, &iobuf->buffer[0], (size_t)IO_BUFFER_SIZE, 0)) <= 0) {
			// an error occurred
			return -1;
		}
		iobuf->level = (size_t)r;

		// align data to the end of the buffer if less than IO_BUFFER_SIZE bytes were read
		memmove(iobuf->buffer + (IO_BUFFER_SIZE - iobuf->level), iobuf->buffer, iobuf->level);
	}

	return 0;
}

int HTTPServer::HTTPServerClient::_readline(iobuffer* iobuf, void* buffer, size_t len, int timeout) {
	char c = '\0', *out = (char*) buffer;
	size_t i;

	memset(buffer, 0, len);

	for (i = 0; i < len && c != '\n'; ++i) {
		if (_read(iobuf, &c, (size_t)1, timeout) <= 0) {
			// timeout or error occured
			return -1;
		}
		*out++ = c;
	}

	return (int)i;
}

void HTTPServer::HTTPServerClient::send_snapshot() {
	uint8_t *frame;
	char buffer[BUFFER_SIZE] = { 0 };

	/* wait for a fresh frame */
	pthread_mutex_lock(&(mServer->Mutex));
	mServer->waitForDataBufferUpdate();

	/* read buffer */
	size_t frame_size = mServer->getDataBufferSize();

	/* allocate a buffer for this single frame */
	if ((frame = (uint8_t*)malloc(frame_size + 1)) == NULL) {
		pthread_mutex_unlock(&(mServer->Mutex));
		send_error(500, "not enough memory");
		return;
	}

	uint8_t* db = mServer->getDataBuffer();
	if (db != NULL) {
		memcpy(frame, db, frame_size);
	}
	Debugger::DEBUG("HTTPServer", "got frame (size: %u kB)", frame_size / 1024);

	pthread_mutex_unlock(&(mServer->Mutex));

	/* write the response */
	sprintf(buffer, "HTTP/1.0 200 OK\r\n"
	STD_HEADER
	"Content-type: image/jpeg\r\n"
	"\r\n");

	/* send header and image now */
	if (send(mSocket, buffer, strlen(buffer), 0) < 0) {
		free(frame);
		return;
	}
	send(mSocket, (char*)frame, frame_size, 0);

	free(frame);
}

void HTTPServer::HTTPServerClient::send_stream() {
	uint8_t *frame = NULL, *tmp = NULL;
	size_t frame_size = 0, max_frame_size = 0;
	char buffer[BUFFER_SIZE] = { 0 };

	Debugger::DEBUG("HTTPServer", "preparing header");

	sprintf(buffer, "HTTP/1.0 200 OK\r\n"
	STD_HEADER
	"Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
	"\r\n"
	"--" BOUNDARY "\r\n");

	if (send(mSocket, buffer, strlen(buffer), 0) < 0) {
		free(frame);
		return;
	}

	Debugger::DEBUG("HTTPServer", "Headers send, sending stream now");

	while (mServer->IsRunning()) {
		// wait for fresh frames
		pthread_mutex_lock(&(mServer->Mutex));
		mServer->waitForDataBufferUpdate();

		// read buffer
		frame_size = mServer->getDataBufferSize();

		// check if framebuffer is large enough, increase it if necessary
		if (frame_size > max_frame_size) {
			Debugger::DEBUG("HTTPServer", "increasing buffer size to %d", frame_size);

			max_frame_size = frame_size + TEN_K;
			if ((tmp = (uint8_t*) realloc(frame, max_frame_size)) == NULL) {
				//lint -e{449} frame is still valid if realloc returned NULL and has to be freed
				free(frame);
				pthread_mutex_unlock(&(mServer->Mutex));
				send_error(500, "not enough memory");
				return;
			}

			frame = tmp;
		}

		uint8_t* db = mServer->getDataBuffer();
		if (frame != NULL && db != NULL) {
			memcpy(frame, db, frame_size);
		}
		//Debugger::DEBUG("HTTPServer", "got frame (size: %d kB)", frame_size / 1024);

		mServer->setClientRequest(true);

		pthread_mutex_unlock(&(mServer->Mutex));

		/*
		 * print the individual mimetype and the length
		 * sending the content-length fixes random stream disruption observed
		 * with firefox
		 */
		sprintf(buffer, "Content-Type: image/jpeg\r\n"
				"Content-Length: %lu\r\n"
				"\r\n", (unsigned long)frame_size);
		//Debugger::DEBUG("HTTPServer", "sending intermediate header");
		if (send(mSocket, buffer, strlen(buffer), 0) < 0) {
			Debugger::WARN("HTTPServer", "sending header failed");
			break;
		}

		//Debugger::DEBUG("HTTPServer", "sending frame");
		if (send(mSocket, (char*)frame, frame_size, 0) < 0) {
			Debugger::WARN("HTTPServer", "sending frame failed");
			break;
		}

		//Debugger::DEBUG("HTTPServer", "sending boundary");
		sprintf(buffer, "\r\n--" BOUNDARY "\r\n");
		if (send(mSocket, buffer, strlen(buffer), 0) < 0) {
			Debugger::WARN("HTTPServer", "sending boundary failed");
			break;
		}
	}

	free(frame);
}

void HTTPServer::HTTPServerClient::send_file(const char *parameter) {
	char buffer[BUFFER_SIZE] = { 0 };
	const char *extension, *mimetype = NULL;
	int i;
	int lfd;

	/* find file-extension */
	if ((extension = strstr((parameter == NULL || strlen(parameter)) ? "index.html" : parameter, ".")) == NULL) {
		send_error(400, "No file extension found");
		return;
	}

	/* determine mime-type */
	for (i = 0; i < (int)LENGTH_OF(mimetypes); ++i) {
		if (strcmp(mimetypes[i].dot_extension, extension) == 0) {
			mimetype = (const char *)mimetypes[i].mimetype;
			break;
		}
	}

	/* in case of unknown mimetype or extension leave */
	if (mimetype == NULL) {
		send_error(404, "MIME-TYPE not known");
		return;
	}

	// now filename, mimetype and extension are known
	Debugger::DEBUG("HTTPServer", "trying to serve file \"%s\", extension: \"%s\" mime: \"%s\"", (parameter == NULL || strlen(parameter)) ? "index.html" : parameter, extension, mimetype);

	// build the absolute path to the file
	strncat(buffer, mServer->getWWWFolder(), sizeof(buffer) - 1);
	strncat(buffer, (parameter == NULL || strlen(parameter)) ? "index.html" : parameter, (sizeof(buffer) - strlen(buffer)) - 1);

	// try to open that file
	if ((lfd = open(buffer, O_RDONLY)) < 0) {
		Debugger::WARN("HTTPServer", "file %s not accessible", buffer);
		send_error(404, "Could not open file");
		return;
	}
	Debugger::DEBUG("HTTPServer", "opened file: %s", buffer);

	// prepare HTTP header
	sprintf(buffer, "HTTP/1.0 200 OK\r\n"
			"Content-type: %s\r\n"
	STD_HEADER
	"\r\n", mimetype);
	ssize_t l = (ssize_t)strlen(buffer);

	// first transmit HTTP-header, afterwards transmit content of file
	do {
		if (send(mSocket, buffer, (size_t)l, 0) < 0) {
			close(lfd);
			return;
		}
	} while ((l = read(lfd, buffer, sizeof(buffer))) > 0);

	// close file, job done
	close(lfd);
}

void HTTPServer::HTTPServerClient::send_error(int which, const char *message) {
	char buffer[BUFFER_SIZE] = { 0 };

	if (which == 401) {
		sprintf(buffer, "HTTP/1.0 401 Unauthorized\r\n"
				"Content-type: text/plain\r\n"
		STD_HEADER
		"WWW-Authenticate: Basic realm=\"MJPG-Streamer\"\r\n"
		"\r\n"
		"401: Not Authenticated!\r\n"
		"%s", message);
	} else if (which == 404) {
		sprintf(buffer, "HTTP/1.0 404 Not Found\r\n"
				"Content-type: text/plain\r\n"
		STD_HEADER
		"\r\n"
		"404: Not Found!\r\n"
		"%s", message);
	} else if (which == 500) {
		sprintf(buffer, "HTTP/1.0 500 Internal Server Error\r\n"
				"Content-type: text/plain\r\n"
		STD_HEADER
		"\r\n"
		"500: Internal Server Error!\r\n"
		"%s", message);
	} else if (which == 400) {
		sprintf(buffer, "HTTP/1.0 400 Bad Request\r\n"
				"Content-type: text/plain\r\n"
		STD_HEADER
		"\r\n"
		"400: Not Found!\r\n"
		"%s", message);
	} else {
		sprintf(buffer, "HTTP/1.0 501 Not Implemented\r\n"
				"Content-type: text/plain\r\n"
		STD_HEADER
		"\r\n"
		"501: Not Implemented!\r\n"
		"%s", message);
	}

	send(mSocket, buffer, strlen(buffer), 0);
}

void HTTPServer::HTTPServerClient::decodeBase64(char *data) const {
	const uint8_t *in = (const uint8_t *) data;
	// The decoded size will be at most 3/4 the size of the encoded
	unsigned ch = 0;
	int i = 0;

	while (*in) {
		uint32_t t = *in++;

		if (t >= '0' && t <= '9')
			t = (t - '0') + 52;
		else if (t >= 'A' && t <= 'Z')
			t = t - 'A';
		else if (t >= 'a' && t <= 'z')
			t = (t - 'a') + 26;
		else if (t == '+')
			t = 62;
		else if (t == '/')
			t = 63;
		else if (t == '=')
			t = 0;
		else
			continue;

		ch = (ch << 6) | t;
		i++;
		if (i == 4) {
			*data++ = (char) (ch >> 16);
			*data++ = (char) (ch >> 8);
			*data++ = (char) ch;
			i = 0;
		}
	}
	*data = '\0';
}

void HTTPServer::HTTPServerClient::free_request() {
	free(mRequestParameter);
	mRequestParameter = NULL;
	free(mRequestClient);
	mRequestClient = NULL;
	free(mRequestCredentials);
	mRequestCredentials = NULL;
}
