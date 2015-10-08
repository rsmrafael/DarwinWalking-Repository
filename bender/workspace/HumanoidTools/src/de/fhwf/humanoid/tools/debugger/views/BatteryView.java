package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.Timer;

import de.fhwf.humanoid.tools.debugger.Debugger;
import de.fhwf.humanoid.tools.debugger.services.BodyDataService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class BatteryView extends JPanel implements ActionListener {
	private static final long serialVersionUID = 853786661766191410L;
	
	private static final int BATTERY_IMAGE_LEFT_OFFSET = 25;
	private static final int BATTERY_IMAGE_RIGHT_OFFSET = 200;
	private static final double BATTERY_MAX_VOLTAGE = (4.2 * 3);
	private static final double BATTERY_MIN_VOLTAGE = (3.4 * 3);
	
	private BodyDataService mBodyDataService;
	private JPanel mBatteryImage;
	private BufferedImage mBatteryIcon;
	private BufferedImage mBatteryIconGray;
	private double mBatteryVoltage;
	private Timer mUpdateTimer;
	private JButton mStartStopButton;

	public BatteryView() {		
		mBodyDataService = (BodyDataService)ServiceManager.getInstance().getService(BodyDataService.SERVICENAME);

		this.setLayout(new BorderLayout());
		
		Icon icon = Debugger.loadIcon("battery.png");
		mBatteryIcon = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_RGB);
		Graphics g = mBatteryIcon.getGraphics();
		g.setColor(this.getBackground());
		g.fillRect(0, 0, icon.getIconWidth(), icon.getIconHeight());
		g.dispose();
		icon.paintIcon(this, mBatteryIcon.createGraphics(), 0, 0);
		// Create grayscale copy
		mBatteryIconGray = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_BYTE_GRAY);  
		g = mBatteryIconGray.getGraphics();  
		g.drawImage(mBatteryIcon, 0, 0, null);  
		g.dispose(); 
		
		mBatteryImage = new JPanel() {
			private static final long serialVersionUID = -6121139138385952066L;

			@Override
			public void paint(Graphics g) {
				int height = mBatteryIcon.getHeight();
				int width = mBatteryIcon.getWidth();
				
				double percent = (100.d / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * (mBatteryVoltage - BATTERY_MIN_VOLTAGE);
				if (percent < 0) {
					percent = 0;
				} else if (percent > 100) {
					percent = 100;
				}
				int percentWidth = (int)(((BATTERY_IMAGE_RIGHT_OFFSET - BATTERY_IMAGE_LEFT_OFFSET) / 100.0d) * percent);
				
				// Draw empty (gray) battery
				g.drawImage(mBatteryIconGray, 0, 0, null);
				// Overlay "charged" part (color)
				g.drawImage(mBatteryIcon, BATTERY_IMAGE_LEFT_OFFSET, 0, BATTERY_IMAGE_LEFT_OFFSET + percentWidth, height, BATTERY_IMAGE_LEFT_OFFSET, 0, BATTERY_IMAGE_LEFT_OFFSET + percentWidth, height, null);
				
				g.setFont(g.getFont().deriveFont(22.0f));
				g.setColor(Color.RED);
				String str = Double.toString(mBatteryVoltage) + " V";
				int strWidth = g.getFontMetrics().stringWidth(str);
				g.drawString(str, (width - strWidth) /  2, (height / 2) + 10); 
			}
		};
		this.add(BorderLayout.CENTER, mBatteryImage);
		
	  mStartStopButton = new JButton("Start");
	  mStartStopButton.setActionCommand("start");
	  mStartStopButton.addActionListener(this);
	  this.add(BorderLayout.SOUTH, mStartStopButton);
	  
	  mUpdateTimer = new Timer(1000, this);
	  mUpdateTimer.setActionCommand("refresh");
	  mUpdateTimer.setRepeats(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getActionCommand().equals("refresh")) {
			try {
				mBatteryVoltage = mBodyDataService.getBatteryVoltage();
				mBatteryImage.repaint();
			} catch (IOException e1) {
				System.err.println("Battery data transfer exception: " + e1.getMessage());
			} catch (IllegalStateException e1) {
				System.err.println("Battery data transfer exception: " + e1.getMessage());
			}
						
		} else if (e.getActionCommand().equals("start")) {
			mUpdateTimer.start();
			mStartStopButton.setText("Stop");
			mStartStopButton.setActionCommand("stop");
		} else if (e.getActionCommand().equals("stop")) {
			mUpdateTimer.stop();
			mStartStopButton.setText("Start");
			mStartStopButton.setActionCommand("start");
		}
	}
}
