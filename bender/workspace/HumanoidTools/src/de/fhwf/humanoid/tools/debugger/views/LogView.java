package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FileDialog;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.net.InetAddress;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JToggleButton;
import javax.swing.SwingUtilities;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import de.fhwf.humanoid.tools.debugger.Debugger;
import de.fhwf.humanoid.tools.debugger.data.LogEntry;
import de.fhwf.humanoid.tools.debugger.events.LogEvent;
import de.fhwf.humanoid.tools.debugger.events.LogListener;
import de.fhwf.humanoid.tools.debugger.services.LogService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.tools.CheckCombo;

public class LogView extends JPanel implements LogListener, ActionListener,
		PopupMenuListener {
	private static final long serialVersionUID = 3306294500438240118L;
	private static final int LOG_MODULE_WIDTH = 200;

	private LogService mLogs;
	private JTable mLoggingTable;
	private DefaultTableModel mLoggingTableModel;
	private Icon mErrorIcon;
	private Icon mWarningIcon;
	private Icon mInfoIcon;
	private Icon mDebugIcon;
	private JToggleButton mLogErrors;
	private JToggleButton mLogWarnings;
	private JToggleButton mLogInfos;
	private JToggleButton mLogDebugs;
	private CheckCombo mLogModule;
	private String mLogModuleText;
	private JButton mClearLogs;
	private JButton mClearAllLogs;
	private JToggleButton mFreeze;
	private JButton mSaveLog;
	private JButton mLoadLog;

	public LogView() {
		mErrorIcon = Debugger.loadIcon("error.png");
		mWarningIcon = Debugger.loadIcon("warning.png");
		mInfoIcon = Debugger.loadIcon("info.png");
		mDebugIcon = Debugger.loadIcon("debug.png");

		this.setLayout(new BorderLayout());

		JPanel filterPanel = new JPanel();
		filterPanel.setLayout(new GridLayout(2, 1));

		JPanel categoriesPanel = new JPanel();
		categoriesPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		categoriesPanel.add(new JLabel("Categories to log: "));
		mLogErrors = new JToggleButton("Errors", mErrorIcon, true);
		mLogErrors.addActionListener(this);
		categoriesPanel.add(mLogErrors);
		mLogWarnings = new JToggleButton("Warnings", mWarningIcon, true);
		mLogWarnings.addActionListener(this);
		categoriesPanel.add(mLogWarnings);
		mLogInfos = new JToggleButton("Information", mInfoIcon, true);
		mLogInfos.addActionListener(this);
		categoriesPanel.add(mLogInfos);
		mLogDebugs = new JToggleButton("Debugging", mDebugIcon, true);
		mLogDebugs.addActionListener(this);
		categoriesPanel.add(mLogDebugs);
		filterPanel.add(categoriesPanel);

		JPanel moduleFilterPanel = new JPanel();
		moduleFilterPanel.setLayout(new FlowLayout(FlowLayout.LEADING));
		moduleFilterPanel.add(new JLabel("Filter by Module:"));
		mLogModule = new CheckCombo();
		mLogModule.setEditable(true);
		mLogModule.setActionCommand("setModule");
		mLogModule.addActionListener(this);
		mLogModule.addPopupMenuListener(this);
		mLogModule.setPreferredSize(new Dimension(LOG_MODULE_WIDTH, 22));
		moduleFilterPanel.add(mLogModule);
		filterPanel.add(moduleFilterPanel);
		this.add(filterPanel, BorderLayout.NORTH);

		JPanel actionsPanel = new JPanel();
		actionsPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		mFreeze = new JToggleButton("Freeze update",
				Debugger.loadIcon("freeze.png"), false);
		mFreeze.addActionListener(this);
		actionsPanel.add(mFreeze);
		mClearLogs = new JButton("Clear", Debugger.loadIcon("clear.png"));
		mClearLogs.addActionListener(this);
		actionsPanel.add(mClearLogs);
		mClearAllLogs = new JButton("Clear all", Debugger.loadIcon("clear.png"));
		mClearAllLogs.addActionListener(this);
		actionsPanel.add(mClearAllLogs);
		mSaveLog = new JButton("Save all", Debugger.loadIcon("save.png"));
		mSaveLog.addActionListener(this);
		actionsPanel.add(mSaveLog);
		mLoadLog = new JButton("Load", Debugger.loadIcon("open.png"));
		mLoadLog.addActionListener(this);
		actionsPanel.add(mLoadLog);
		this.add(actionsPanel, BorderLayout.SOUTH);

		String columnNames[] = { "", "Level", "Time", "Module", "Message" };
		mLoggingTableModel = new DefaultTableModel(columnNames, 0);
		mLoggingTable = new JTable(mLoggingTableModel) {
			private static final long serialVersionUID = 1L;

			@Override
			public String getToolTipText(MouseEvent e) {
				Point p = e.getPoint();
				int row = mLoggingTable.rowAtPoint(p);
				int col = mLoggingTable.columnAtPoint(p);
				if (row >= 0) {
					row = mLoggingTable.convertRowIndexToModel(row);
					col = mLoggingTable.convertColumnIndexToModel(col);
					if (col == 4) {
						Object v = mLoggingTable.getValueAt(row, 4);
						return v.toString();
					} else {
						return null;
					}
				} else {
					return null;
				}
			}
		};
		DefaultTableCellRenderer r = new DefaultTableCellRenderer() {
			private static final long serialVersionUID = 1L;

			public void setValue(Object value) {
				if (value instanceof Icon) {
					setIcon((Icon) value);
					setText("");
				} else {
					setIcon(null);
					super.setValue(value);
				}
			}
		};
		mLoggingTable.setDefaultRenderer(Object.class, r);
		mLoggingTable.setFillsViewportHeight(true);
		mLoggingTable.getColumnModel().getColumn(0).setMinWidth(24);
		mLoggingTable.getColumnModel().getColumn(0).setMaxWidth(24);
		mLoggingTable.getColumnModel().getColumn(1).setPreferredWidth(58);
		mLoggingTable.getColumnModel().getColumn(1).setMaxWidth(58);
		mLoggingTable.getColumnModel().getColumn(2).setPreferredWidth(58);
		mLoggingTable.getColumnModel().getColumn(2).setMaxWidth(58);
		mLoggingTable.getColumnModel().getColumn(3).setPreferredWidth(114);
		mLoggingTable.getColumnModel().getColumn(3).setMaxWidth(114);
		mLoggingTable.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
		this.add(new JScrollPane(mLoggingTable), BorderLayout.CENTER);

		mLogs = (LogService) ServiceManager.getInstance().getService(
				LogService.SERVICENAME);
		mLogs.addLogListener(this);

		mLogModuleText = null;
		mLogModule.getEditor().setItem("< All modules >");
	}

	@Override
	public void newLog(LogEvent e) {
		Icon icon = null;
		LogEntry log = e.getLog();
		if (!mFreeze.isSelected()) {
			if (displayLogType(log)) {
				String logText = log.getModule().toLowerCase();
				if (mLogModuleText == null
						|| mLogModuleText.contains("," + logText + ",")) {
					switch (log.getLevel()) {
					case ERROR:
						icon = mErrorIcon;
						break;
					case WARN:
						icon = mWarningIcon;
						break;
					case INFO:
						icon = mInfoIcon;
						break;
					case DEBUG:
						icon = mDebugIcon;
						break;
					}
					Object[] rowData = { icon, log.getLevel(),
							log.getLogTime(), log.getModule(), log.getMessage() };
					mLoggingTableModel.insertRow(0, rowData);
				}
			}
		}
	}

	private void updateLogs() {
		// while (mLoggingTableModel.getRowCount() > 0) {
		// mLoggingTableModel.removeRow(0);
		// }
		String columnNames[] = { "", "Level", "Time", "Module", "Message" };
		mLoggingTableModel = new DefaultTableModel(columnNames, 0);
		mLoggingTable.setModel(mLoggingTableModel);
		mLoggingTable.getColumnModel().getColumn(0).setMinWidth(24);
		mLoggingTable.getColumnModel().getColumn(0).setMaxWidth(24);
		mLoggingTable.getColumnModel().getColumn(1).setPreferredWidth(58);
		mLoggingTable.getColumnModel().getColumn(1).setMaxWidth(58);
		mLoggingTable.getColumnModel().getColumn(2).setPreferredWidth(58);
		mLoggingTable.getColumnModel().getColumn(2).setMaxWidth(58);
		mLoggingTable.getColumnModel().getColumn(3).setPreferredWidth(114);
		mLoggingTable.getColumnModel().getColumn(3).setMaxWidth(114);

		List<LogEntry> logs = mLogs.getLogs();
		synchronized (logs) {
			for (LogEntry log : logs) {
				newLog(new LogEvent(this, log));
			}
		}
	}

	private boolean displayLogType(LogEntry log) {
		switch (log.getLevel()) {
		case ERROR:
			return mLogErrors.isSelected();
		case WARN:
			return mLogWarnings.isSelected();
		case INFO:
			return mLogInfos.isSelected();
		case DEBUG:
			return mLogDebugs.isSelected();
		}
		return true;
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(mFreeze)) {
			if (mFreeze.isSelected()) {
				return;
			}
		}
		if (e.getSource().equals(mClearLogs)) {
			mLogs.clear();
		}
		if (e.getSource().equals(mClearAllLogs)) {
			if (JOptionPane.showConfirmDialog(this,
					"Are you sure you want to clean the logs of all robots?",
					"Clean all logs", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
				mLogs.clearAllLogs();
			}
		}
		if (e.getSource().equals(mLogModule)) {
			List<Object> modules = mLogModule.getSelectedItems();
			mLogModuleText = ",";
			String logModules = "";
			for (Object m : modules) {
				if (m != null) {
					String module = (String) m;
					if (module.equals("< All modules >")) {
						mLogModuleText = null;
						break;
					} else {
						mLogModuleText += module.toLowerCase() + ",";
						logModules += module + ", ";
					}
				} else {
					mLogModuleText = null;
				}
			}
			if (mLogModuleText != null) {
				if (logModules.length() > 2) {
					logModules = logModules.substring(0,
							logModules.length() - 2);
				}
				final String logModulesFin = logModules;
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mLogModule.getEditor().setItem(logModulesFin);
					}
				});
			} else {
				final String logModulesFin = "< All modules >";
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mLogModule.getEditor().setItem(logModulesFin);
					}
				});
			}
		}
		if (e.getSource().equals(mSaveLog)) {
			FileDialog filed = new FileDialog((Frame) null, "Save log",
					FileDialog.SAVE);
			DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd-HHmm");
			java.util.Date date = new java.util.Date();
			String name = "LOG-" + dateFormat.format(date);
			filed.setFile(name + ".txt");
			filed.setVisible(true);
			String filename = filed.getFile();
			if (filename != null) {
				filename = filed.getDirectory() + filename;
				try {
					mLogs.saveAll(filename);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(this, e1.getMessage(),
							"Log save exception", JOptionPane.ERROR_MESSAGE,
							null);
				}
			}
			filed.dispose();
		}
		if (e.getSource().equals(mLoadLog)) {
			FileDialog filed = new FileDialog((Frame) null, "Load log",
					FileDialog.LOAD);
			filed.setVisible(true);
			String filename = filed.getFile();
			if (filename != null) {
				filename = filed.getDirectory() + filename;
				try {
					mLogs.load(filename);
					List<InetAddress> addr = mLogs.getSourceAddresses();
					String msg = "Logs loaded from following IPs:\n\n";
					for (InetAddress a : addr) {
						msg += a.getHostAddress() + "\n";
					}
					JOptionPane.showMessageDialog(this, msg, "Log loaded",
							JOptionPane.INFORMATION_MESSAGE, null);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(this, e1.getMessage(),
							"Log save exception", JOptionPane.ERROR_MESSAGE,
							null);
				}
			}
			filed.dispose();
		}
		updateLogs();
	}

	@Override
	public void popupMenuCanceled(PopupMenuEvent e) {
	}

	@Override
	public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
	}

	@Override
	public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
		Set<String> existingModules = new HashSet<String>();
		for (int i = 0; i < mLogModule.getItemCount(); ++i) {
			existingModules.add(mLogModule.getItemAt(i).toString());
		}

		SortedSet<String> modules = new TreeSet<String>();
		modules.add("< All modules >");
		for (LogEntry log : mLogs.getLogs()) {
			modules.add(log.getModule());
		}

		// Remove vanished items
		for (String log : existingModules) {
			if (!modules.contains(log)) {
				mLogModule.removeItem(log);
			}
		}

		// Add new items
		for (String log : modules) {
			if (!existingModules.contains(log)) {
				mLogModule.addItem(log);
			}
		}
	}
}
