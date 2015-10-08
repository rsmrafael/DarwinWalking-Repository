package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.RowSorter;
import javax.swing.SortOrder;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;
import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;

import de.fhwf.humanoid.tools.debugger.blackboard.representations.FieldLinesRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.HistogrammRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.IRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.PointCloudRepresentation;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.UnknownRepresentationTypeException;
import de.fhwf.humanoid.tools.debugger.blackboard.representations.YUVImageRepresentation;
import de.fhwf.humanoid.tools.debugger.services.BlackboardService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.ostfalia.robocup.message.MsgBlackboard.Blackboard;
import de.ostfalia.robocup.message.MsgBlackboard.Blackboard.Representation;

public class BlackboardView extends JPanel {
	private static final long serialVersionUID = -2028555909336718266L;

	private BlackboardService mBlackboardService;
	private JTable mBlackboardTable;
	private DefaultTableModel mBlackboardTableModel;
	private Blackboard mBlackboard;

	public BlackboardView() {
		this.setLayout(new BorderLayout());

		mBlackboardService = (BlackboardService) ServiceManager.getInstance()
				.getService(BlackboardService.SERVICENAME);

		String columnNames[] = { "Name", "Type" };
		mBlackboardTableModel = new DefaultTableModel(columnNames, 0) {
			private static final long serialVersionUID = 1L;

			@Override
			public boolean isCellEditable(int row, int column) {
				return false;
			}
		};
		mBlackboardTable = new JTable(mBlackboardTableModel);
		mBlackboardTable.setFillsViewportHeight(true);
		mBlackboardTable.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				if (e.getClickCount() == 2) {
					JTable target = (JTable) e.getSource();
					int row = target.getSelectedRow();
					int column = target.getSelectedColumn();
					if (column == 1) {
						showRepresentation(
								mBlackboardTableModel.getValueAt(row, 0)
										.toString(), e.getLocationOnScreen().x,
								e.getLocationOnScreen().y);
					}
				}
			}
		});

		TableRowSorter<TableModel> sorter = new TableRowSorter<TableModel>(
				mBlackboardTableModel);
		List<RowSorter.SortKey> sortKeys = new ArrayList<RowSorter.SortKey>();
		sortKeys.add(new RowSorter.SortKey(0, SortOrder.ASCENDING));
		sorter.setSortKeys(sortKeys);
		mBlackboardTable.setRowSorter(sorter);

		this.add(new JScrollPane(mBlackboardTable), BorderLayout.CENTER);

		JPanel bottomPanel = new JPanel(new FlowLayout(FlowLayout.LEADING));
		JButton read = new JButton("Read from robot");
		read.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				try {
					mBlackboard = mBlackboardService.getTOC();
					showBlackboardContents();
				} catch (IOException e) {
					JOptionPane.showMessageDialog(null, e.getMessage(),
							"Blackboard exception", JOptionPane.ERROR_MESSAGE,
							null);
				}
			}
		});
		bottomPanel.add(read);
		this.add(bottomPanel, BorderLayout.SOUTH);
	}

	void showBlackboardContents() {
		mBlackboardTableModel.setRowCount(0);
		if (mBlackboard != null) {
			for (Representation r : mBlackboard.getRepresentationsList()) {
				mBlackboardTableModel.addRow(new String[] { r.getName(),
						r.getType() });
			}
		}
	}

	void showRepresentation(String name, int x, int y) {
		// When there's no blackboard, try to download it
		if (mBlackboard == null) {
			try {
				mBlackboard = mBlackboardService.getAll();
			} catch (IOException ex) {
				JOptionPane
						.showMessageDialog(null, ex.getMessage(),
								"Blackboard exception",
								JOptionPane.ERROR_MESSAGE, null);
				return;
			}
		}

		// Is the actual representation data available?
		Representation representationMsg = null;
		for (Representation r : mBlackboard.getRepresentationsList()) {
			if (r.getName().equals(name)) {
				// Representation found - but is the data there?
				if (!r.getData().isEmpty()) {
					representationMsg = r;
				}
				break;
			}
		}

		if (representationMsg == null) {
			// Representation not found or empty - Get complete Blackboard
			try {
				mBlackboard = mBlackboardService.getAll();
				for (Representation r : mBlackboard.getRepresentationsList()) {
					if (r.getName().equals(name)) {
						// Representation found - but is the data there?
						if (!r.getData().isEmpty()) {
							representationMsg = r;
						}
						break;
					}
				}
			} catch (IOException ex) {
				JOptionPane
						.showMessageDialog(null, ex.getMessage(),
								"Blackboard exception",
								JOptionPane.ERROR_MESSAGE, null);
				return;
			}
		}

		if (representationMsg == null) {
			// Representation still not found - something is wrong...
			JOptionPane
					.showMessageDialog(
							null,
							"Failed to download representation "
									+ name
									+ " from Robot!\nRepresentation not existing or not serialized!",
							"Blackboard exception", JOptionPane.ERROR_MESSAGE,
							null);
			return;
		}

		// De-serialize representation
		try {
			final IRepresentation rep = deserializeRepresentationData(
					representationMsg.getType(), representationMsg.getData());

			if (rep.getVisualizer().size() > 1) {
				final JPopupMenu menu = new JPopupMenu();
				ActionListener menuListener = new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						menu.setVisible(false);
						rep.showEditor(event.getActionCommand());
					}
				};

				menu.setLocation(x, y);

				for (String s : rep.getVisualizer()) {
					JMenuItem item = new JMenuItem(s);
					item.addActionListener(menuListener);
					menu.add(item);
				}

				menu.setVisible(true);

			} else if (rep.getVisualizer().size() == 1) {
				rep.showEditor(rep.getVisualizer().get(0));
			} else {
				JOptionPane.showMessageDialog(null,
						" No Visualizer registered!", "Blackboard exception",
						JOptionPane.ERROR_MESSAGE, null);
			}

		} catch (InvalidProtocolBufferException e) {
			/**JOptionPane.showMessageDialog(null, e.getMessage(),
					"Blackboard exception", JOptionPane.ERROR_MESSAGE, null);*/
			e.printStackTrace();
		} catch (UnknownRepresentationTypeException e) {
			JOptionPane.showMessageDialog(null, e.getMessage(),
					"Blackboard exception", JOptionPane.ERROR_MESSAGE, null);
		}

	}

	private IRepresentation deserializeRepresentationData(String type,
			ByteString data) throws InvalidProtocolBufferException,
			UnknownRepresentationTypeException {
		if (type.equals("10DataHolderI8YUVImageE") || type.equals("10DataHolderI11OpenCVImageE") ) {
			return new YUVImageRepresentation(data);
		} else if (type.equals("10DataHolderI10PointCloudE")) {
			return new PointCloudRepresentation(data);
		} else if (type.equals("10DataHolderI10FieldLinesE")) {
			return new FieldLinesRepresentation(data);
		} else if (type.equals("10DataHolderI10HistogrammE")) {
			return new HistogrammRepresentation(data);
		}
		throw new UnknownRepresentationTypeException(
				"Unknown representation type: " + type);
	}
}