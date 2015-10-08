package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import de.fhwf.humanoid.tools.debugger.Debugger;
import de.fhwf.humanoid.tools.debugger.events.StatusEvent;
import de.fhwf.humanoid.tools.debugger.events.StatusListener;
import de.fhwf.humanoid.tools.debugger.services.ControlService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class ActiveBotsView extends JPanel implements StatusListener {
	private static final long serialVersionUID = 8374154388714263741L;

	private static String[] botNames = { "Jonny", "Locutus", "", "Copper", "Hugh" };
	private static int UPDATE_INTERVAL = 1000;
	private static int INACTIVE_TIMEOUT = 30;
	
	private class Bot {
		public InetAddress IP;
		public long lastSeen;
		public int ID;
		public String activeAI;
		public String goalColor;
		public boolean isAllowedToMove;
		
		public Bot(InetAddress ip, String statusString) {
			IP = ip;
			update(statusString);
		}
		
		public void update(String statusString) {
			String status[] = statusString.split("\\|");
			lastSeen = System.currentTimeMillis();
			if (status.length >= 3) {
				ID = Integer.parseInt(status[0]);
				activeAI = status[1];
				goalColor = status[2];
				isAllowedToMove = Boolean.parseBoolean(status[3]);
			}
		}

		public Object[] getRowData() {
			int inactive = (int)(System.currentTimeMillis() - lastSeen) / 1000;
			String inactiveStr = Integer.toString(inactive);
		  if (inactive == 1) {
		  	inactiveStr += " second ago";
		  } else {
		  	inactiveStr += " seconds ago";
		  }
			
		  String botName = "";
		  if (ID > 0 && ID <= botNames.length) {
		  	botName += " (" + botNames[ID - 1] + ")";
		  }
		  
  		Object[] rowData = { ID + botName, IP.getHostAddress(), inactiveStr, activeAI, goalColor, isAllowedToMove ? "No" : "Yes" };
  		return rowData;
		}
	}
	
	private ControlService mControl;
	private JTable mBotsTable;
	private DefaultTableModel mBotsTableModel;
	private Map<InetAddress, Bot> mActiveBots;

	public ActiveBotsView() {
		this.setLayout(new BorderLayout());

		mActiveBots = new HashMap<InetAddress, Bot>();
		
		String columnNames[] = { "ID", "IP", "Last Seen", "AI", "Goal color", "Paralysed" };
		mBotsTableModel = new DefaultTableModel(columnNames, 0) {
			private static final long serialVersionUID = 1L;

			@Override
			public boolean isCellEditable(int row, int column) {
	        return false;
	    }			
		};
		mBotsTable = new JTable(mBotsTableModel) {
			private static final long serialVersionUID = 1L;

			@Override
			public String getToolTipText(MouseEvent e) {
        Point p = e.getPoint();
        int row = mBotsTable.rowAtPoint(p); 
        int col = mBotsTable.columnAtPoint(p);
        if (row >= 0) {
          row = mBotsTable.convertRowIndexToModel(row);
          col = mBotsTable.convertColumnIndexToModel(col);
          if (col == 3) {
          	Object v = mBotsTableModel.getValueAt(row, 3);
          	return v.toString();
  				} else {
  					return null;
  				}
        } else {
        	return null;
        }
			}
		};
		mBotsTable.setFillsViewportHeight(true);
		DefaultTableCellRenderer r = new DefaultTableCellRenderer() {
			private static final long serialVersionUID = 1L;

			public void setValue(Object value) {
				if (value instanceof String) {
					if (((String)value).contains("second")) {
						int v = Integer.parseInt(((String)value).split(" ")[0]);
						if (v > INACTIVE_TIMEOUT) {
							setBackground(Color.RED);
						} else {
							setBackground(null);
						}
					} else {
						setBackground(null);
					}
				} else {
					setBackground(null);
				}
				setForeground(Color.BLACK);
				super.setValue(value);
			}
		};
		mBotsTable.setDefaultRenderer(Object.class, r);
		mBotsTable.getColumnModel().getColumn(0).setMinWidth(60);
    mBotsTable.getColumnModel().getColumn(0).setMaxWidth(60);
		mBotsTable.getColumnModel().getColumn(1).setMinWidth(120);
    mBotsTable.getColumnModel().getColumn(1).setMaxWidth(120);
		mBotsTable.getColumnModel().getColumn(2).setMinWidth(100);
    mBotsTable.getColumnModel().getColumn(2).setMaxWidth(100);
		mBotsTable.getColumnModel().getColumn(4).setMinWidth(60);
    mBotsTable.getColumnModel().getColumn(4).setMaxWidth(60);
		mBotsTable.getColumnModel().getColumn(5).setMinWidth(60);
    mBotsTable.getColumnModel().getColumn(5).setMaxWidth(60);
		this.add(new JScrollPane(mBotsTable), BorderLayout.CENTER);
		
		mControl = (ControlService)ServiceManager.getInstance().getService(ControlService.SERVICENAME);
		mControl.addStatusListener(this);

		Timer updateTimer = new Timer();
	  TimerTask tt = new TimerTask() {
	   public void run() {
	    updateTable();
	   }
	  };
	  updateTimer.schedule(tt, 0, UPDATE_INTERVAL);  
	  
	  mBotsTable.addMouseListener(new MouseAdapter() {
	      public void mouseClicked(MouseEvent e) {
	          if (e.getComponent().isEnabled() && e.getButton() == MouseEvent.BUTTON1 && e.getClickCount() == 2) {
	              Point p = e.getPoint();
	              int row = mBotsTable.rowAtPoint(p); 
	              // Use table.convertRowIndexToModel / table.convertColumnIndexToModle to convert to view indices
	              row = mBotsTable.convertRowIndexToModel(row);
	              Object v = mBotsTableModel.getValueAt(row, 1);
	              if (v instanceof String) {
	              	Debugger.getInstance().connectTo((String)v);
	              }
	          }
	      }
	  });
	}

	@Override
	public void newStatus(StatusEvent e) {
		InetAddress ip = e.getSender();
		if (mActiveBots.containsKey(ip)) {
			Bot bot = mActiveBots.get(ip);
			bot.update(e.getStatus());
		} else {
			Bot bot = new Bot(e.getSender(), e.getStatus());
			mActiveBots.put(ip, bot);
		}
		updateTable();
	}
	
	private void updateTable() {
		int row = mBotsTable.getSelectedRow();
		
		mBotsTableModel.setNumRows(0);
		for (Bot bot : mActiveBots.values()) {
			mBotsTableModel.insertRow(0, bot.getRowData());
		}
		
		if (row >= 0 && mBotsTableModel.getRowCount() > row) {
			mBotsTable.setRowSelectionInterval(row, row);
		}
	}
}