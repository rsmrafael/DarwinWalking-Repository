package de.fhwf.humanoid.tools.debugger.services;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.event.EventListenerList;

import de.fhwf.humanoid.tools.debugger.connection.PacketEvent;
import de.fhwf.humanoid.tools.debugger.connection.PacketListener;
import de.fhwf.humanoid.tools.debugger.data.LogEntry;
import de.fhwf.humanoid.tools.debugger.events.LogEvent;
import de.fhwf.humanoid.tools.debugger.events.LogListener;

public class LogService implements IService, PacketListener {
	public static final String SERVICENAME = "LogService";
	private static final Pattern PATTERN = Pattern.compile("<(\\d+)> \\[(.*)\\] (.*)");
	private static final Pattern LOAD_PATTERN = Pattern.compile("(.*) - (.*) <(\\d+)> \\[(.*)\\] (.*)");
	
	private List<LogEntry> mAllLogs;
	private List<LogEntry> mCurrentSourceLogs;
	private LogEntry mLastLog;
	private EventListenerList mListeners = new EventListenerList();
	private InetAddress mCurrentSourceAddress;
	
	public LogService() {
		mAllLogs = new LinkedList<LogEntry>();
		mCurrentSourceLogs = new LinkedList<LogEntry>();
		mCurrentSourceAddress = null;
	}
	
	@Override
	public void newPacket(PacketEvent e) {
		String logs[] = e.getPacket().split("\n");
		for (int i = 0; i < logs.length; i++) {
			Matcher m = PATTERN.matcher(logs[i]);
			if (m.find()) {
				int level = Integer.parseInt(m.group(1));
				String module = m.group(2);
				String message = m.group(3);
				if (mLastLog != null) {
		 			synchronized (mAllLogs) {
		 				mAllLogs.add(mLastLog);
		 			}
					if (e.getSender().equals(mCurrentSourceAddress)) {
			 			synchronized (mCurrentSourceLogs) {
			 				mCurrentSourceLogs.add(mLastLog);
			 			}				
		  			notifyNewLog(new LogEvent(this, mLastLog));
					}
					mLastLog = null;
				}
				mLastLog = new LogEntry(level, module, message, e.getSender());
			} else {
				mLastLog.append(logs[i]);
			}
		}
		if (mLastLog != null) {
 			synchronized (mAllLogs) {
 				mAllLogs.add(mLastLog);
 			}
			if (e.getSender().equals(mCurrentSourceAddress)) {
	 			synchronized (mCurrentSourceLogs) {
	 				mCurrentSourceLogs.add(mLastLog);
	 			}				
  			notifyNewLog(new LogEvent(this, mLastLog));
			}
			mLastLog = null;
		}
	}

	@Override
	public String getPacketID() {
		return "LOG";
	}
	
	public void clear() {
		synchronized (mCurrentSourceLogs) {
			mCurrentSourceLogs.clear();
		}
	}

	public void clearAllLogs() {
		synchronized (mCurrentSourceLogs) {
			mCurrentSourceLogs.clear();
		}
		synchronized (mAllLogs) {
			mAllLogs.clear();
		}		
	}
	
	public List<LogEntry> getLogs() {
		return mCurrentSourceLogs;
	}

	public List<LogEntry> getAllLogs() {
		return mAllLogs;
	}

	public void saveAll(String filename) throws IOException {
		File outputfile = new File(filename);
		BufferedWriter BW = new BufferedWriter(new FileWriter(outputfile));
		
		List<LogEntry> logs = getAllLogs();
		synchronized (logs) {
  		for (LogEntry log : logs) {
  			BW.write(log.toString() + "\n");
  		}
		}
		
		BW.close();
	}

	public void load(String filename) throws IOException {
		File inputfile = new File(filename);
		BufferedReader BR = new BufferedReader(new FileReader(inputfile));
		
		clearAllLogs();
		clear();
		synchronized (mAllLogs) {
			String line = null;
			while ((line = BR.readLine()) != null) {
				Matcher m = LOAD_PATTERN.matcher(line);
				if (m.find()) {
					InetAddress source = InetAddress.getByName( m.group(1));
					String time = m.group(2);
					long timeMs = 0;
					
					DateFormat df = new SimpleDateFormat("HH:mm:ss.SSS");
					try {
						Date timeD = df.parse(time);
						timeMs = timeD.getTime();
					} catch (ParseException e) {
					}
					
					//buf.append(mLogReceivedTime % 1000);
					
					int level = Integer.parseInt(m.group(3));
					String module = m.group(4);
					String message = m.group(5);
					LogEntry log = new LogEntry(level, module, message, source, timeMs);
					mAllLogs.add(log);
				}
			}
		}
		
		BR.close();
		
		setCurrentSourceAddress(mCurrentSourceAddress);
	}

	public void setCurrentSourceAddress(InetAddress source) {
		mCurrentSourceAddress = source;
		synchronized (mCurrentSourceLogs) {
			mCurrentSourceLogs.clear();
			for (LogEntry log : mAllLogs) {
				String host = log.getSource().getHostAddress();
				String host2 = mCurrentSourceAddress.getHostAddress();
				if (host.equals(host2)) {
					mCurrentSourceLogs.add(log);
				}
			}
		}		
	}
	
	public List<InetAddress> getSourceAddresses() {
		List<InetAddress> list = new LinkedList<InetAddress>();

		List<LogEntry> logs = getAllLogs();
		synchronized (logs) {
  		for (LogEntry log : logs) {
  			if (!list.contains(log.getSource())) {
  				list.add(log.getSource());
  			}
  		}
		}
		
		return list;
	}

	public void addLogListener(LogListener listener) {
		mListeners.add(LogListener.class, listener);
	}

	public void removeLogListener(LogListener listener) {
		mListeners.remove(LogListener.class, listener);
	}

	private synchronized void notifyNewLog(LogEvent event) {
		for (LogListener l : mListeners.getListeners(LogListener.class)) {
			l.newLog(event);
		}
	}

	@Override
	public String getServiceName() {
		return SERVICENAME;
	}
	
	@Override
	public boolean supportsMultipleSources() {
		return true;
	}
}
