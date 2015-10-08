package de.fhwf.humanoid.tools.debugger.data;

import java.net.InetAddress;
import java.sql.Time;

public class LogEntry {
	public enum LogLevel {
		DEBUG(4), INFO(3), WARN(2), ERROR(1);

		private final int mID;

		LogLevel(int id) {
			mID = id;
		}

		public int id() {
			return mID;
		}
	};
	
	private LogLevel mLevel;
	private String mModule;
	private String mMessage;
	private InetAddress mSource;
	private long mLogReceivedTime;

	public LogEntry(int level, String module, String message, InetAddress source) {
		this(level, module, message, source, System.currentTimeMillis());
	}
	
	public LogEntry(int level, String module, String message, InetAddress source, long time) {
		switch(level) {
			case 1:
				mLevel = LogLevel.ERROR;
				break;
			case 2:
				mLevel = LogLevel.WARN;
				break;
			case 3:
				mLevel = LogLevel.INFO;
				break;
			case 4:
				mLevel = LogLevel.DEBUG;
				break;
		}
		mModule = module;
		mMessage = message;
		mSource = source;
		mLogReceivedTime = time;
	}
	
	public void append(String message) {
		mMessage = mMessage + '\n' + message;
	}
	
	public LogLevel getLevel() {
		return mLevel;
	}
	
	public String getModule() {
		return mModule;
	}
	
	public String getMessage() {
		return mMessage;
	}

	public InetAddress getSource() {
		return mSource;
	}
	
	public String getLogTime() {
		return new Time(mLogReceivedTime).toString();
	}
	
	@Override
	public String toString() {
		StringBuffer buf = new StringBuffer(256);
		if (mSource != null) {
			buf.append(mSource.getHostAddress());
			buf.append(" - ");
		}
		buf.append(new Time(mLogReceivedTime).toString());
		buf.append('.');
		buf.append(mLogReceivedTime % 1000);
		buf.append(" <");
		buf.append(mLevel.mID);
		buf.append("> [");
		buf.append(mModule);
		buf.append("] ");
		buf.append(mMessage);
		return buf.toString();
	}
}
