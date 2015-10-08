package de.fhwf.humanoid.tools.debugger.blackboard.representations;

import java.awt.Color;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartFrame;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.DefaultCategoryDataset;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;

import de.ostfalia.robocup.message.MsgHistogramm.Histogramm;
import de.ostfalia.robocup.message.MsgHistogramm.Histogramm.Data;

public class HistogrammRepresentation extends IRepresentation {

	public HashMap<Double, Integer> mMap = new HashMap<Double, Integer>();

	public HistogrammRepresentation(ByteString data)
			throws InvalidProtocolBufferException {

		Histogramm hist = de.ostfalia.robocup.message.MsgHistogramm.Histogramm
				.parseFrom(data);

		for (int i = 0; i < hist.getHistCount(); ++i) {
			Data blob = hist.getHist(i);

			mMap.put(blob.getKey(), blob.getValue());

		}

		registerVisualizer("List");
		registerVisualizer("Image Overlay");
	}

	@Override
	public void serialize() {
		// TODO Auto-generated method stub

	}

	@Override
	public void showEditor(String visualizer) {

		DefaultCategoryDataset dataset = new DefaultCategoryDataset();

		Iterator<Entry<Double, Integer>> it = mMap.entrySet().iterator();
		while (it.hasNext()) {
			Entry<Double, Integer> entry = (Entry<Double, Integer>) it.next();
			dataset.setValue(entry.getValue(), "Marks", entry.getKey());
		}

		JFreeChart chart = ChartFactory.createBarChart(
				"BarChart using JFreeChart", "Student", "Marks", dataset,
				PlotOrientation.VERTICAL, false, true, false);
		chart.setBackgroundPaint(Color.yellow);
		chart.getTitle().setPaint(Color.blue);
		CategoryPlot p = chart.getCategoryPlot();
		p.setRangeGridlinePaint(Color.red);
		ChartFrame frame1 = new ChartFrame("Bar Chart", chart);
		frame1.setVisible(true);
		frame1.setSize(400, 350);

	}
}
