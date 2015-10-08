package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fhwf.humanoid.tools.debugger.services.ControlService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class CameraSettings extends JPanel implements ActionListener, ChangeListener {
	private static final long serialVersionUID = 7436854876102641195L;
	
	private ControlService mControlService;
	private JComboBox mCmbResolution;
	private JSlider mSlidGain;
	private JSlider mSlidShutter;
	private JComboBox mSlidWhiteBalance;
	private JSlider mSlidColorSaturation;
	private JSlider mSlidBrigthness;
	private JSlider mSlidContrast;
	private JButton mCmdRead;
	private boolean mUpdating;
	
	public CameraSettings() {
		mControlService = (ControlService)ServiceManager.getInstance().getService(ControlService.SERVICENAME);
		this.setLayout(new BorderLayout());

		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));
		

		JPanel cont = new JPanel();
		cont.add(new JLabel("Resolution:"));
		mCmbResolution = new JComboBox(new String[]{"1600x1200", "1280x1024", "1024x768", "800x600", "640x480", "352x288", "320x240", "176x144"});
		mCmbResolution.addActionListener(this);
		cont.add(mCmbResolution);
		centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Gain:"));
		mSlidGain = new JSlider(0, 31, 0);
		mSlidGain.setSnapToTicks(true);
		mSlidGain.addChangeListener(this);
		cont.add(mSlidGain);
		centerPanel.add(cont);
		
		cont = new JPanel();
		cont.add(new JLabel("Shutter:"));
		mSlidShutter = new JSlider(-2, 2, 0);
		mSlidShutter.setSnapToTicks(true);
		mSlidShutter.addChangeListener(this);
		cont.add(mSlidShutter);
		centerPanel.add(cont);
		
		cont = new JPanel();
		cont.add(new JLabel("White Balance:"));
		mSlidWhiteBalance = new JComboBox(new String[]{"Auto", "Sunny", "Cloudy", "Office", "Home"});
		mSlidWhiteBalance.addActionListener(this);
		cont.add(mSlidWhiteBalance);
		centerPanel.add(cont);
		
		cont = new JPanel();
		cont.add(new JLabel("Saturation:"));
		mSlidColorSaturation = new JSlider(-2, 2, 0);
		mSlidColorSaturation.setSnapToTicks(true);
		mSlidColorSaturation.addChangeListener(this);
		cont.add(mSlidColorSaturation);
		centerPanel.add(cont);

		cont = new JPanel();
		cont.add(new JLabel("Brightness:"));
		mSlidBrigthness = new JSlider(-2, 2, 0);
		mSlidBrigthness.setSnapToTicks(true);
		mSlidBrigthness.addChangeListener(this);
		cont.add(mSlidBrigthness);
		centerPanel.add(cont);
		
		cont = new JPanel();
		cont.add(new JLabel("Contrast:"));
		mSlidContrast = new JSlider(-2, 2, 0);
		mSlidContrast.setSnapToTicks(true);
		mSlidContrast.addChangeListener(this);
		cont.add(mSlidContrast);
		centerPanel.add(cont);	
		
		this.add(centerPanel);
		
		mCmdRead = new JButton("Read from robot");
		mCmdRead.addActionListener(this);
		this.add(mCmdRead, BorderLayout.SOUTH);
		
		mUpdating = false;
	}

	@Override
	public void stateChanged(ChangeEvent e) {
	 if (mUpdating) {
	  return;
	 }
		try {
			 if (e.getSource().equals(mSlidGain)) {
				 mControlService.setSetting("GAIN ", mSlidGain.getValue()+"" );
			 } else if (e.getSource().equals(mSlidShutter)) {
				 mControlService.setSetting("SHUTT", mSlidShutter.getValue()+"");
			 }  else if (e.getSource().equals(mSlidColorSaturation)) {				 
				 mControlService.setSetting("SATUR", mSlidColorSaturation.getValue()+"");				
			 } else if (e.getSource().equals(mSlidBrigthness)) {				 
				 mControlService.setSetting("BRIGH", mSlidBrigthness.getValue()+"");				
			 } else if (e.getSource().equals(mSlidContrast)) {				 
				 mControlService.setSetting("CONTR", mSlidContrast.getValue()+"");				
			 }
		} catch (IOException e1) {
			 JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
		 }
	}

	@Override
	public void actionPerformed(ActionEvent e) {
  if (mUpdating) {
   return;
  }
		try {
		if (e.getSource().equals(mCmdRead)) {
				  Map<String, String> settings = mControlService.getSettings();
				  mUpdating = true;
				  for (Entry<String, String> s : settings.entrySet()) {
					  String key = s.getKey();
					  if (key.equals("RESUL")) {
						  mCmbResolution.setSelectedItem(settings.get(key));
					  } else if (key.equals("GAIN")) {
						  mSlidGain.setValue(Integer.valueOf(settings.get(key)));
					  } else if (key.equals("SHUTT")) {
						  mSlidShutter.setValue(Integer.valueOf(settings.get(key)));
					  } else if (key.equals("WHITE")) {
						  mSlidWhiteBalance.setSelectedIndex(Integer.valueOf(settings.get(key)));
					  } else if (key.equals("SATUR")) {
						  mSlidColorSaturation.setValue(Integer.valueOf(settings.get(key)));
					  } else if (key.equals("BRIGH")) {
						  mSlidBrigthness.setValue(Integer.valueOf(settings.get(key)));
					  } else if (key.equals("CONTR")) {
						  mSlidContrast.setValue(Integer.valueOf(settings.get(key)));
					  }
				  }
				  mUpdating = false;
			 } else if (e.getSource().equals(mSlidWhiteBalance)) {
				 mControlService.setSetting("WHITE", mSlidWhiteBalance.getSelectedIndex()+"");
			 } else if (e.getSource().equals(mCmbResolution)) { 
				 mControlService.setSetting("RESUL", mCmbResolution.getSelectedItem().toString());
			 } else {
				System.out.println("[Error] unknown Source"); 
				try {
					throw new Exception();
				} catch (Exception e1) {
					e1.printStackTrace();
				}
			 } 
		 } catch (IOException e1) {
			 JOptionPane.showMessageDialog(null, e1.getMessage(), "Bot control exception", JOptionPane.ERROR_MESSAGE, null);
		 }
	}
}
