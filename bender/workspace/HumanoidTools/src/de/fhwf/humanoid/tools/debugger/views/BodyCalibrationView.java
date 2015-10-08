package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fhwf.humanoid.tools.debugger.data.PIDParameters;
import de.fhwf.humanoid.tools.debugger.services.BodyCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.services.BodyCalibrationService.PidID;

public class BodyCalibrationView extends JPanel implements ActionListener, ChangeListener {
	private static final long serialVersionUID = -1444416162305804714L;

	private BodyCalibrationService mBodyService;
	private JComboBox mPIDSelector;
	private JSlider mPSlider;
	private JLabel mPLabel;
	private JSlider mISlider;
	private JLabel mILabel;
	private JSlider mDSlider;
	private JLabel mDLabel;
	private JSlider mSPSlider;
	private JLabel mSPLabel;
	private JButton mSetButton;
	private PidID mSelectedPid;
	
	public BodyCalibrationView() {
		mBodyService = (BodyCalibrationService)ServiceManager.getInstance().getService(BodyCalibrationService.SERVICENAME);
		
		this.setLayout(new BorderLayout());
		
		mPIDSelector = new JComboBox();
		mPIDSelector.addItem("< Select PID controller >");
		PidID[] pids = PidID.values();
		for (int i = 0; i < pids.length; i++) {
			mPIDSelector.addItem(pids[i]);
		}
		mPIDSelector.addActionListener(this);
		this.add(mPIDSelector, BorderLayout.NORTH);
						
		JPanel sliders = new JPanel();
		sliders.setLayout(new GridLayout(2, 2));
		JPanel row = new JPanel();
		mPSlider = new JSlider(0, 2000);
		mPSlider.addChangeListener(this);
		mPSlider.setMinorTickSpacing(100);
		mPSlider.setPaintTicks(true);
		mPSlider.setSnapToTicks(true);
		mPSlider.setEnabled(false);
		mPSlider.setPreferredSize(new Dimension(150, 20));
		mPLabel = new JLabel("---");
		row.add(new JLabel("P:"));
		row.add(mPSlider);
		row.add(mPLabel);
		sliders.add(row);

		row = new JPanel();
		mISlider = new JSlider(0, 2000);
		mISlider.addChangeListener(this);
		mISlider.setMinorTickSpacing(100);
		mISlider.setPaintTicks(true);
		mISlider.setSnapToTicks(true);
		mISlider.setEnabled(false);
		mISlider.setPreferredSize(new Dimension(150, 20));
		mILabel = new JLabel("---");
		row.add(new JLabel("I:"));
		row.add(mISlider);
		row.add(mILabel);
		sliders.add(row);
		
		row = new JPanel();
		mDSlider = new JSlider(0, 2000);
		mDSlider.setMinorTickSpacing(100);
		mDSlider.setPaintTicks(true);
		mDSlider.setSnapToTicks(true);
		mDSlider.addChangeListener(this);
		mDSlider.setEnabled(false);
		mDSlider.setPreferredSize(new Dimension(150, 20));
		mDLabel = new JLabel("---");
		row.add(new JLabel("D:"));
		row.add(mDSlider);
		row.add(mDLabel);
		sliders.add(row);

		row = new JPanel();
		mSPSlider = new JSlider(0, 50000);
		mSPSlider.setMinorTickSpacing(100);
		mSPSlider.setPaintTicks(true);
		mSPSlider.setSnapToTicks(true);
		mSPSlider.addChangeListener(this);
		mSPSlider.setEnabled(false);
		mSPSlider.setPreferredSize(new Dimension(150, 20));
		mSPLabel = new JLabel("---");
		row.add(new JLabel("Set point:"));
		row.add(mSPSlider);
		row.add(mSPLabel);
		sliders.add(row);

		this.add(sliders, BorderLayout.CENTER);
		
		mSetButton = new JButton("Set");
		mSetButton.addActionListener(this);
		mSetButton.setEnabled(false);
		this.add(mSetButton, BorderLayout.SOUTH);
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		if (e.getSource().equals(mPSlider)) {
			mPLabel.setText(String.valueOf(mPSlider.getValue() / 1000.0f));
		} else if (e.getSource().equals(mISlider)) {
			mILabel.setText(String.valueOf(mISlider.getValue() / 1000.0f));
		} else if (e.getSource().equals(mDSlider)) {
			mDLabel.setText(String.valueOf(mDSlider.getValue() / 1000.0f));
		} else if (e.getSource().equals(mSPSlider)) {
			mSPLabel.setText(String.valueOf(mSPSlider.getValue() / 1000.0f));
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(mPIDSelector)) {
			if (mPIDSelector.getSelectedItem() instanceof PidID) {
				PidID pid = (PidID)mPIDSelector.getSelectedItem();
				try {
					mSelectedPid = pid;
					PIDParameters selparams = mBodyService.getPIDParameters(pid);
					mPSlider.setValue((int)(selparams.P * 1000d));
					mISlider.setValue((int)(selparams.I * 1000d));
					mDSlider.setValue((int)(selparams.D * 1000d));
					mSPSlider.setValue((int)(selparams.setPoint * 1000d));
					mPSlider.setEnabled(true);
					mISlider.setEnabled(true);
					mDSlider.setEnabled(true);
					mSPSlider.setEnabled(true);
					mSetButton.setEnabled(true);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(this, e1.getMessage(), "Body calibration exception", JOptionPane.ERROR_MESSAGE, null);
				}
			} else {
				mPSlider.setEnabled(false);
				mISlider.setEnabled(false);
				mDSlider.setEnabled(false);
				mSPSlider.setEnabled(false);
				mSetButton.setEnabled(false);
			}
		} else if (e.getSource().equals(mSetButton)) {
			try {
				double p = mPSlider.getValue() / 1000d;
				double i = mISlider.getValue() / 1000d;
				double d = mDSlider.getValue() / 1000d;
				double sp = mSPSlider.getValue() / 1000d;
				PIDParameters parameters = new PIDParameters(p, i, d, sp);
				mBodyService.setPIDParameters(mSelectedPid, parameters);
			} catch (IOException e1) {
				JOptionPane.showMessageDialog(this, e1.getMessage(), "Body calibration exception", JOptionPane.ERROR_MESSAGE, null);			
			}
		}
	}
}
