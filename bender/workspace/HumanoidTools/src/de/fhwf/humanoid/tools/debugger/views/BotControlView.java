package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Map;
import java.util.Map.Entry;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import de.fhwf.humanoid.tools.debugger.services.ControlService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class BotControlView extends JPanel implements ActionListener, ChangeListener {
	private static final long serialVersionUID = 7436854876102641195L;

	private ControlService mControlService;
	private JComboBox mCmbAIName;
 private JComboBox mCmbTempAIName;
	private JComboBox mCmbBotID;
	private JComboBox mCmbTeamColor;
	private JComboBox mCmbGoalColor;
	private JCheckBox mChkAllowedToMove;
	private JButton mCmdRead;
	private boolean mSuppressChangeEvent;
	
	public BotControlView() {
		mControlService = (ControlService)ServiceManager.getInstance().getService(ControlService.SERVICENAME);
		
		this.setLayout(new BorderLayout());

		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));
		
		JPanel cont = new JPanel();
		cont.add(new JLabel("Selected AI:"));
		mCmbAIName = new JComboBox(new String[]{""});
		mCmbAIName.setActionCommand("ai");
  mCmbAIName.addActionListener(this);
		cont.add(mCmbAIName);
		centerPanel.add(cont);
		
  cont = new JPanel();
  cont.add(new JLabel("Temporary AI:"));
  mCmbTempAIName = new JComboBox(new String[]{""});
  mCmbTempAIName.setActionCommand("tempai");
  mCmbTempAIName.addActionListener(this);
  cont.add(mCmbTempAIName);
  JButton reset = new JButton("Reset");
  reset.setActionCommand("resettempai");
  reset.addActionListener(this);
  cont.add(reset);
  centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Robot ID:"));
		mCmbBotID = new JComboBox(new String[]{"1", "2", "3", "4", "5"});
		mCmbBotID.setActionCommand("id");
  mCmbBotID.addActionListener(this);
		cont.add(mCmbBotID);
		centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Team color:"));
		mCmbTeamColor = new JComboBox(new String[]{"Cyan", "Magenta"});
		mCmbTeamColor.setActionCommand("teamcolor");
  mCmbTeamColor.addActionListener(this);
		cont.add(mCmbTeamColor);
		centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Goal color:"));
		mCmbGoalColor = new JComboBox(new String[]{"Blue", "Yellow"});
		mCmbGoalColor.setActionCommand("goalcolor");
  mCmbGoalColor.addActionListener(this);
		cont.add(mCmbGoalColor);
		centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Robot is allowed to move:"));
		mChkAllowedToMove = new JCheckBox("", true);
		mChkAllowedToMove.setActionCommand("move");
		mChkAllowedToMove.addActionListener(this);
		cont.add(mChkAllowedToMove);
		centerPanel.add(cont);

		
		this.add(centerPanel);
		
		mCmdRead = new JButton("Read from robot");
		mCmdRead.setActionCommand("get");
		mCmdRead.addActionListener(this);
		this.add(mCmdRead, BorderLayout.SOUTH);
		
		setReadState(false);
		mSuppressChangeEvent = false;
	}

	private void setReadState(boolean state) {
	 mCmbAIName.setEnabled(state);
	 mCmbTempAIName.setEnabled(state);
	 mCmbBotID.setEnabled(state);
	 mCmbTeamColor.setEnabled(state);
	 mCmbGoalColor.setEnabled(state);
	}
	
	@Override
	public void stateChanged(ChangeEvent e) {
	}

	@Override
	public void actionPerformed(ActionEvent e) {
	 if (e.getActionCommand().equals("get")) {
	  try {
    mSuppressChangeEvent = true;
	   Map<String, String> settings = mControlService.getSettings();
	   for (Entry<String, String> s : settings.entrySet()) {
	    String key = s.getKey();
	    if (key.equals("GOALCOLOR")) {
	     if (s.getValue().equalsIgnoreCase("yellow")) {
	      mCmbGoalColor.setSelectedItem("Yellow");
	     } else if (s.getValue().equalsIgnoreCase("blue")) {
	      mCmbGoalColor.setSelectedItem("Blue");
	     }
	    } else if (key.equals("TMCOLOR")) {
      if (s.getValue().equalsIgnoreCase("cyan")) {
       mCmbTeamColor.setSelectedItem("Cyan");
      } else if (s.getValue().equalsIgnoreCase("magenta")) {
       mCmbTeamColor.setSelectedItem("Magenta");
      }
     } else if (key.equals("BOTID")) {
      mCmbBotID.setSelectedItem(s.getValue());
     } else if (key.equals("AI")) {
      mCmbAIName.setSelectedItem(s.getValue().toUpperCase());
     } else if (key.equals("TEMPAI")) {
      mCmbTempAIName.setSelectedItem(s.getValue().toUpperCase());
     } else if (key.equals("MOVE")) {
      mChkAllowedToMove.setSelected(s.getValue().equals("1"));
     } else if (key.equals("AVAILAI")) {
      mCmbAIName.removeAllItems();
      mCmbTempAIName.removeAllItems();
      mCmbTempAIName.addItem("");
      String[] AIs = s.getValue().split(";");
      for (String ai : AIs) {
       mCmbAIName.addItem(ai.toUpperCase());
       mCmbTempAIName.addItem(ai.toUpperCase());
      }
     }
	   }
	   setReadState(true);
    mSuppressChangeEvent = false;
	  } catch (IOException e1) {
	   JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
	  }
	 } else if (e.getActionCommand().equals("move")) {
	  if (mSuppressChangeEvent) { return; }
	  try {
	   mControlService.setSetting("MOVE ", mChkAllowedToMove.isSelected() ? "1" : "0");
	  } catch (IOException e1) {
	   JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
	  }
  } else if (e.getActionCommand().equals("id")) {
   if (mSuppressChangeEvent) { return; }
   try {
    mControlService.setSetting("BOTID", mCmbBotID.getSelectedItem().toString());
   } catch (IOException e1) {
    JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
   }
  } else if (e.getActionCommand().equals("teamcolor")) {
   if (mSuppressChangeEvent) { return; }
   try {
    mControlService.setSetting("TMCOL", mCmbTeamColor.getSelectedItem().toString().toUpperCase());
   } catch (IOException e1) {
    JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
   }
  } else if (e.getActionCommand().equals("goalcolor")) {
   if (mSuppressChangeEvent) { return; }
   try {
    mControlService.setSetting("GOCOL", mCmbGoalColor.getSelectedItem().toString().toUpperCase());
   } catch (IOException e1) {
    JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
   }
  } else if (e.getActionCommand().equals("ai")) {
   if (mSuppressChangeEvent) { return; }
   try {
    mControlService.setSetting("AI   ", mCmbAIName.getSelectedItem().toString().toUpperCase());
   } catch (IOException e1) {
    JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
   }
  } else if (e.getActionCommand().equals("tempai")) {
   if (mSuppressChangeEvent) { return; }
   try {
    mControlService.setSetting("TMPAI", mCmbTempAIName.getSelectedItem().toString().toUpperCase());
   } catch (IOException e1) {
    JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
   }
  } else if (e.getActionCommand().equals("resettempai")) {
   mCmbTempAIName.setSelectedItem("");
	 }
	}
}
