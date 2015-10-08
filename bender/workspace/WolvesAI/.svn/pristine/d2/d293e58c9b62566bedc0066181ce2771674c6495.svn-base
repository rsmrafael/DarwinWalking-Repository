/*
 * HTTPServer.h
 *
 *  Created on: 19.01.2013
 *      Author: Stefan Krupop
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "Network.h"
#include "../Thread.h"

#define IO_BUFFER_SIZE	256

class HTTPServer : public Thread {
public:
	HTTPServer(uint16_t port, size_t dataBufferSize, const char* wwwFolder = NULL, const char* credentials = NULL);
	virtual ~HTTPServer();

	bool getClientRequest() const;
	void setClientRequest(bool state);
	uint8_t* getDataBuffer();
	void setDataBufferSize(size_t size);
	void notifyDataBufferUpdate();

	pthread_mutex_t Mutex;

protected:
	const char* getCredentials() const;
	const char* getWWWFolder() const;
	void waitForDataBufferUpdate();
	size_t getDataBufferSize() const;

private:
	class HTTPServerClient : public Thread {
	public:
		HTTPServerClient(HTTPServer* server, SOCKET clientSocket);
		virtual ~HTTPServerClient();

	private:
		enum answer {
			A_UNKNOWN, A_SNAPSHOT, A_STREAM, A_COMMAND, A_FILE
		};

		typedef struct {
			size_t level; /* how full is the buffer */
			char buffer[IO_BUFFER_SIZE]; /* the data */
		} iobuffer;

		struct mimetype_s {
			const char *dot_extension;
			const char *mimetype;
		};

		//lint -e(1704)
		HTTPServerClient(const HTTPServerClient& cSource);
		HTTPServerClient& operator=(const HTTPServerClient& cSource);

		void execute(void* arg);
		int _read(iobuffer* iobuf, void* buffer, size_t len, int timeout);
		int _readline(iobuffer* iobuf, void* buffer, size_t len, int timeout);
		void send_snapshot();
		void send_stream();
		void send_file(const char *parameter);
		void send_error(int which, const char *message);
		void decodeBase64(char *data) const;
		void free_request();

		HTTPServer* mServer;
		SOCKET mSocket;
		answer mRequestType;
		char* mRequestParameter;
		char* mRequestClient;
		char* mRequestCredentials;

		static const mimetype_s mimetypes[];
	};

	//lint -e(1704)
	HTTPServer(const HTTPServer& cSource);
	HTTPServer& operator=(const HTTPServer& cSource);

	void execute(void* arg);

	SOCKET mSD;
	uint16_t mPort;
	const char* mWWWFolder;
	const char* mCredentials;
	bool mClientRequest;

	pthread_cond_t mCondBufferUpdated;

	// global JPG frame, this is more or less the "database"
	uint8_t* mDataBuffer;
	size_t mDataBufferSize;

};

#endif /* HTTPSERVER_H_ */
