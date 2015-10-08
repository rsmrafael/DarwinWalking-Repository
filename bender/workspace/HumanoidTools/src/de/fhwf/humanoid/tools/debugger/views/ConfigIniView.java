package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.RowSorter;
import javax.swing.SortOrder;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;
import org.ini4j.Ini;
import de.fhwf.humanoid.tools.debugger.services.ControlService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class ConfigIniView extends JPanel {
 private static final long serialVersionUID = -215394403434280168L;

 private ControlService mControl;
	private JTable mSettingsTable;
	private DefaultTableModel mSettingsTableModel;

	public ConfigIniView() {
		this.setLayout(new BorderLayout());

  mControl = (ControlService)ServiceManager.getInstance().getService(ControlService.SERVICENAME);

		String columnNames[] = { "Section", "Key", "Value" };
		mSettingsTableModel = new DefaultTableModel(columnNames, 0) {
			private static final long serialVersionUID = 1L;

			@Override
			public boolean isCellEditable(int row, int column) {
	        if (column == 2) {
	         return true;
	        }
	        return false;
	    }			
		};
		mSettingsTable = new JTable(mSettingsTableModel);
		mSettingsTable.setFillsViewportHeight(true);
		
		TableRowSorter<TableModel> sorter = new TableRowSorter<TableModel>(mSettingsTableModel);
  List<RowSorter.SortKey> sortKeys = new ArrayList<RowSorter.SortKey>();
  sortKeys.add(new RowSorter.SortKey(0, SortOrder.ASCENDING));
  sortKeys.add(new RowSorter.SortKey(1, SortOrder.ASCENDING));
  sorter.setSortKeys(sortKeys);
		mSettingsTable.setRowSorter(sorter);
		
		mSettingsTableModel.addTableModelListener(new TableModelListener() {
   @Override
   public void tableChanged(TableModelEvent arg0) {
    if (arg0.getType() == TableModelEvent.UPDATE && 
      arg0.getColumn() != TableModelEvent.ALL_COLUMNS && 
      arg0.getFirstRow() == arg0.getLastRow() && arg0.getColumn() == 2) {
     int row = arg0.getFirstRow();
     String section = mSettingsTableModel.getValueAt(row, 0).toString();
     String key = mSettingsTableModel.getValueAt(row, 1).toString();
     String value = mSettingsTableModel.getValueAt(row, 2).toString();

     try {
      mControl.setConfigIni(section, key, value);
     } catch (IOException e) {
      JOptionPane.showMessageDialog(null, e.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
     }
    }
   }
  });
		
		JLabel help = new JLabel("<html><b>CAUTION:</b> Changing values here might not be picked up by AI before restart!<br>&quot;Save&quot; will instruct robot to save it's own representation, not to send the whole INI displayed below!</html>");
		this.add(help, BorderLayout.NORTH);
		
		this.add(new JScrollPane(mSettingsTable), BorderLayout.CENTER);
		
		JPanel bottomPanel = new JPanel(new FlowLayout(FlowLayout.LEADING));
		JButton read = new JButton("Read from robot");
		read.addActionListener(new ActionListener() {
   @Override
   public void actionPerformed(ActionEvent arg0) {
    try {
     String iniString = mControl.getConfigIni();
     
     Ini ini = new Ini(new StringReader(iniString));
     mSettingsTableModel.setRowCount(0);
     for (String section : ini.keySet()) {
      Map<String, String> map = ini.get(section);
      for (Entry<String, String> entry : map.entrySet()) {
       mSettingsTableModel.addRow(new String[]{section, entry.getKey(), entry.getValue()});
      }
     }
    } catch (IOException e) {
     JOptionPane.showMessageDialog(null, e.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
    }
   }
  });
		bottomPanel.add(read);
  JButton save = new JButton("Save");
  save.addActionListener(new ActionListener() {
   @Override
   public void actionPerformed(ActionEvent arg0) {
    try {
     mControl.saveConfigIni();
    } catch (IOException e) {
     JOptionPane.showMessageDialog(null, e.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
    }
   }
  });
  bottomPanel.add(save);
		this.add(bottomPanel, BorderLayout.SOUTH);
	}
}