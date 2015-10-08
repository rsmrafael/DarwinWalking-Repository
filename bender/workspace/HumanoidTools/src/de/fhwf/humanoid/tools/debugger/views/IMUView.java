package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.Timer;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.RectangleEdge;

import de.fhwf.humanoid.tools.debugger.data.IMUData;
import de.fhwf.humanoid.tools.debugger.services.BodyDataService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

public class IMUView extends JPanel implements ActionListener {
	private static final long serialVersionUID = 853786661766191410L;
	
	private static final int MAX_SAMPLES = 50;
	
	private BodyDataService mBodyDataService;
	private JFreeChart mChart;
	private ChartPanel mChartPanel;
	private XYSeries mDataRoll;
	private XYSeries mDataPitch;
	private long mStartTimeStamp;
	private Timer mUpdateTimer;
	private JButton mStartStopButton;

	public IMUView() {		
		mBodyDataService = (BodyDataService)ServiceManager.getInstance().getService(BodyDataService.SERVICENAME);

		this.setLayout(new BorderLayout());
		
		XYSeriesCollection data = new XYSeriesCollection();
	  mChart = ChartFactory.createXYLineChart("", "Time", "Angle", data, PlotOrientation.VERTICAL, true, false, false);
	  mChart.getLegend().setPosition(RectangleEdge.RIGHT);
	  
	  ValueAxis axis = mChart.getXYPlot().getRangeAxis();
	  axis.setStandardTickUnits(NumberAxis.createIntegerTickUnits());
	  axis.setRange(-180, +180);
	  
	  mDataRoll = new XYSeries("Roll");
	  data.addSeries(mDataRoll);
	  mDataPitch = new XYSeries("Pitch");
	  data.addSeries(mDataPitch);

	  mChart.getXYPlot().getRenderer().setSeriesStroke(0, new BasicStroke(3));
	  mChart.getXYPlot().getRenderer().setSeriesStroke(1, new BasicStroke(3));
	  
	  mChartPanel = new ChartPanel(mChart);
	  mChartPanel.setMouseZoomable(true);
	  this.add(BorderLayout.CENTER, mChartPanel);
	  
	  mStartStopButton = new JButton("Start");
	  mStartStopButton.setActionCommand("start");
	  mStartStopButton.addActionListener(this);
	  this.add(BorderLayout.SOUTH, mStartStopButton);
	  
	  mUpdateTimer = new Timer(100, this);
	  mUpdateTimer.setActionCommand("refresh");
	  mUpdateTimer.setRepeats(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getActionCommand().equals("refresh")) {
			try {
				IMUData data = mBodyDataService.getIMUData();

				if (data != null) {
					mDataRoll.add(System.currentTimeMillis() - mStartTimeStamp, data.Roll);
					mDataPitch.add(System.currentTimeMillis() - mStartTimeStamp, data.Pitch);
				}
			} catch (IOException e1) {
				System.err.println("IMU data transfer exception: " + e1.getMessage());
			}
						
			if (mDataRoll.getItemCount() > MAX_SAMPLES) {
				mDataRoll.remove(0);
				mDataPitch.remove(0);
			}			
		} else if (e.getActionCommand().equals("start")) {
			mDataRoll.clear();
			mDataPitch.clear();
		  mStartTimeStamp = System.currentTimeMillis();
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
