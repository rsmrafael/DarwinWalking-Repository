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

/**
 * a simple HTTP server
 */
class HTTPServer : public Thread {
public:
	/**
	 * Constructor
	 * @param port				port for the connection
	 * @param dataBufferSize	size of the data buffer
	 * @param wwwFolder			www path
	 * @param credentials		credentials
	 */
	HTTPServer(uint16_t port, size_t dataBufferSize, const char* wwwFolder = NULL, const char* credentials = NULL);
	virtual ~HTTPServer();

	/**
	 * check if there is an client request
	 * @return true if client request
	 */
	bool getClientRequest() const;

	/**
	 * set state of client request
	 * @param state		true if client request
	 */
	void setClientRequest(bool state);

	/**
	 * get the data buffer
	 * @return data buffer
	 */
	uint8_t* getDataBuffer();

	/**
	 * set the data buffer size
	 * @param size		new size for the data buffer
	 */
	void setDataBufferSize(size_t size);

	void setBuffer(uint8_t* param) { this->mDataBuffer = param; }

	/**
	 * notify data buffer update
	 */
	void notifyDataBufferUpdate();

	pthread_mutex_t Mutex;		//!< mutex for multi-threading

protected:
	/**
	 * get credentials
	 */
	const char* getCredentials() const;

	/**
	 * get www folder
	 */
	const char* getWWWFolder() const;

	/**
	 * wait for data buffer update
	 */
	void waitForDataBufferUpdate();

	/**
	 * get data buffer size
	 */
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
