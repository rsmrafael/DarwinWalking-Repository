package de.fhwf.humanoid.tools.debugger.visualizer;

import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableModel;

import de.fhwf.humanoid.tools.debugger.blackboard.representations.FieldLinesRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.LineRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.PointCloudRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.PointRepresentation;

public class ListVisualizer extends JScrollPane {

	/**
	 * 
	 */
	private static final long serialVersionUID = -8166510147681704163L;
	private JTable mTable;
	private TableModel mTableModel;

	public ListVisualizer(PointCloudRepresentation pointCloud) {
		super();

		String[] columnNames = { "X", "Y" };
		mTableModel = new DefaultTableModel(columnNames, pointCloud.getSize());

		int counter = 0;

		for (PointRepresentation pr : pointCloud.getCoud()) {
			mTableModel.setValueAt(pr.x, counter, 0);
			mTableModel.setValueAt(pr.y, counter, 1);
			++counter;
		}

		mTable = new JTable(mTableModel);
		this.getViewport().add(mTable);

	}

	public ListVisualizer(FieldLinesRepresentation fieldLines) {
		super();

		String[] columnNames = { "startX", "startY", "endX", "endY" };
		mTableModel = new DefaultTableModel(columnNames, fieldLines.getSize());

		int counter = 0;

		for (LineRepresentation pr : fieldLines.getCoud()) {
			mTableModel.setValueAt(pr.mStartX, counter, 0);
			mTableModel.setValueAt(pr.mStartY, counter, 1);
			mTableModel.setValueAt(pr.mEndX, counter, 2);
			mTableModel.setValueAt(pr.mEndY, counter, 3);
			++counter;
		}

		mTable = new JTable(mTableModel);
		this.getViewport().add(mTable);
	}
}
