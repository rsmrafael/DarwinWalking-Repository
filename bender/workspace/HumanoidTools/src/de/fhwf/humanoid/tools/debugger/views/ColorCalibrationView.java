package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.events.ColorClickEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorClickListener;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorSelectChangeEvent;
import de.fhwf.humanoid.tools.debugger.events.MarkedColorsChangedEvent;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;
import de.fhwf.humanoid.tools.debugger.services.ImageService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.tools.YUVSelector;
import de.fhwf.humanoid.tools.debugger.undoredo.UndoRedoHistory;

public class ColorCalibrationView extends JPanel implements ActionListener,
	ChangeListener, ColorClickListener {
    private static final long serialVersionUID = -1444416162305804714L;

    private ColorCalibrationService mColorService;
    private JComboBox mColorSelector;
    private YUVSelector mYUV;
    private JButton mSendButton;
    private JButton mVisualizeButton;
    private JButton m3DVisualizeButton;
    private JButton mVisualizeStopButton;
    private JButton mClearButton;
    private JButton mGetButton;
    private JButton mSaveToFileButton;
    private JButton mLoadFromFileButton;
    private ColorID mSelectedColor;
    private JButton mUndoButton;
    private JButton mRedoButton;

    public ColorCalibrationView() {
	mColorService = (ColorCalibrationService) ServiceManager.getInstance()
		.getService(ColorCalibrationService.SERVICENAME);

	this.setLayout(new BorderLayout());

	JPanel colorSelectorPanel = new JPanel();

	mColorSelector = new JComboBox();
	ColorID[] colors = ColorID.values();
	for (int i = 0; i < colors.length; i++) {
	    mColorSelector.addItem(colors[i]);
	}
	mSelectedColor = ColorID.Orange;
	mColorSelector.addActionListener(this);
	colorSelectorPanel.add(mColorSelector);

	mVisualizeButton = new JButton("Visualize");
	mVisualizeButton.addActionListener(this);
	mVisualizeButton.setEnabled(true);
	colorSelectorPanel.add(mVisualizeButton);
	
	m3DVisualizeButton = new JButton(new ImageIcon("resources/3dvisu.png"));
	m3DVisualizeButton.addActionListener(this);
	m3DVisualizeButton.setEnabled(true);
	m3DVisualizeButton.setToolTipText("Starts 3D Visualisation");
	colorSelectorPanel.add(m3DVisualizeButton);
	
	mVisualizeStopButton = new JButton(new ImageIcon("resources/stop3dvisu.png"));
	mVisualizeStopButton.addActionListener(this);
	mVisualizeStopButton.setEnabled(true);
	mVisualizeStopButton.setToolTipText("Ends 3D Visualisation");
	colorSelectorPanel.add(mVisualizeStopButton);

	mClearButton = new JButton(new ImageIcon("resources/clear2.png"));
	mClearButton.addActionListener(this);
	mClearButton.setToolTipText("Clear");
	colorSelectorPanel.add(mClearButton);

	mUndoButton = new JButton(new ImageIcon("resources/undo.png"));
	mUndoButton.addActionListener(this);
	mUndoButton.setEnabled(false);
	mUndoButton.setToolTipText("Undo");
	colorSelectorPanel.add(mUndoButton);

	mRedoButton = new JButton(new ImageIcon("resources/redo.png"));
	mRedoButton.addActionListener(this);
	mRedoButton.setEnabled(false);
	mRedoButton.setToolTipText("Redo");
	colorSelectorPanel.add(mRedoButton);

	this.add(colorSelectorPanel, BorderLayout.NORTH);

	mYUV = new YUVSelector(128);
	this.add(mYUV, BorderLayout.CENTER);

	JPanel sliders = new JPanel();

	mGetButton = new JButton("Get Table");
	mGetButton.addActionListener(this);
	sliders.add(mGetButton);

	mSendButton = new JButton("Send Table");
	mSendButton.addActionListener(this);
	mSendButton.setEnabled(true);
	sliders.add(mSendButton);

	mSaveToFileButton = new JButton("Save Table");
	mSaveToFileButton.addActionListener(this);
	mSaveToFileButton.setEnabled(true);
	sliders.add(mSaveToFileButton);

	mLoadFromFileButton = new JButton("Load Table");
	mLoadFromFileButton.addActionListener(this);
	mLoadFromFileButton.setEnabled(true);
	sliders.add(mLoadFromFileButton);

	this.add(sliders, BorderLayout.SOUTH);

	mColorService.addColorClickListener(this);
    }

    @Override
    public void stateChanged(ChangeEvent e) {

    }

    @Override
    public void actionPerformed(ActionEvent e) {
	if (e.getSource().equals(mColorSelector)) {
	    if (mColorSelector.getSelectedItem() instanceof ColorID) {
		ColorID color = (ColorID) mColorSelector.getSelectedItem();
		mSelectedColor = color;
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
			.getInstance().getService(
				ColorCalibrationService.SERVICENAME);
		colorService
			.notifyColorSelectChange(new ColorSelectChangeEvent(
				this, color));
	    } else {
		mSendButton.setEnabled(false);
	    }
	} else if (e.getSource().equals(mSendButton)) {
	    try {
		BufferedWriter BW = new BufferedWriter(new FileWriter(
			"YUVTable.txt"));
		mColorService.getLookUpTable().saveToFile(BW);
		BW.close();
		mColorService.sendColorCalibration();
	    } catch (IOException e1) {
		e1.printStackTrace();
	    }

	} else if (e.getSource().equals(mClearButton)) {
	    ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
		    .getInstance().getService(
			    ColorCalibrationService.SERVICENAME);
	    colorService.notifyMarkedColorsChange(new MarkedColorsChangedEvent(
		    this, MarkedColorsChangedEvent.MChangeType.Cleared,
		    mSelectedColor, null, 1));
	}  else if (e.getSource().equals(mVisualizeButton)) {

	    ImageService imageService = (ImageService) ServiceManager
		    .getInstance().getService(ImageService.SERVICENAME);
	    imageService
		    .notifyColorConfigurationUpdate(new ColorConfigurationUpdateEvent(
			    this, mColorService.getLookUpTable(),
			    mSelectedColor));

	} else if (e.getSource().equals(m3DVisualizeButton)) {
	    ColorCalibrationVisualisation.setUpdate(true);

	    ImageService imageService = (ImageService) ServiceManager
		    .getInstance().getService(ImageService.SERVICENAME);
	    imageService
		    .notifyColorConfigurationUpdate(new ColorConfigurationUpdateEvent(
			    this, mColorService.getLookUpTable(),
			    mSelectedColor));

	} else if (e.getSource().equals(mVisualizeStopButton)) {
	    ColorCalibrationVisualisation.setUpdate(false);
	    ImageService imageService = (ImageService) ServiceManager
		    .getInstance().getService(ImageService.SERVICENAME);
	    imageService
		    .notifyColorConfigurationUpdate(new ColorConfigurationUpdateEvent(
			    this, mColorService.getLookUpTable(),
			    mSelectedColor));

	} else if (e.getSource().equals(mGetButton)) {

	    try {
		mColorService.getColorCalibration();
	    } catch (IOException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	    }
	} else if (e.getSource().equals(mSaveToFileButton)) {
	    FileDialog filed = new FileDialog((Frame) null, "Save LookUpTable",
		    FileDialog.SAVE);
	    DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd-HHmm");
	    java.util.Date date = new java.util.Date();
	    String name = dateFormat.format(date);
	    name += "-";
	    name += "YUVTable";

	    filed.setFile(name + ".txt");
	    filed.setVisible(true);
	    String filename = filed.getFile();
	    if (filename != null) {
		filename = filed.getDirectory() + filename;
		File outputfile = new File(filename);
		try {
		    BufferedWriter BW = new BufferedWriter(new FileWriter(
			    outputfile));
		    mColorService.getLookUpTable().saveToFile(BW);
		    BW.close();
		} catch (IOException e1) {
		    // TODO Auto-generated catch block
		    e1.printStackTrace();
		}
	    }
	    filed.dispose();
	} else if (e.getSource().equals(mLoadFromFileButton)) {
	    FileDialog filed = new FileDialog((Frame) null,
		    "Load ColorCalibration", FileDialog.LOAD);
	    filed.setVisible(true);
	    String fileName = filed.getDirectory() + filed.getFile();
	    try {
		BufferedReader br = new BufferedReader(new InputStreamReader(
			new FileInputStream(fileName)));
		StringBuffer colorBuffer = new StringBuffer();

		String line;
		while ((line = br.readLine()) != null) {
		    colorBuffer.append(line);
		}
		String colorCalibrationData = colorBuffer.toString();

		mColorService.getLookUpTable().updateLookUpTable(
			colorCalibrationData);

		br.close();
	    } catch (IOException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	    }
	    ;
	    filed.dispose();
	    
	} else if (e.getSource().equals(mUndoButton)) {
	    UndoRedoHistory<LookUpTable> history = mColorService
		    .getLookUpTable().getUndoRedoHistory();
	    if (history.canUndo()) {
		history.undo();
	    }
	    mUndoButton.setEnabled(history.canUndo());
	    mRedoButton.setEnabled(history.canRedo());
	} else if (e.getSource().equals(mRedoButton)) {
	    UndoRedoHistory<LookUpTable> history = mColorService
		    .getLookUpTable().getUndoRedoHistory();
	    if (history.canRedo()) {
		history.redo();
	    }
	    mUndoButton.setEnabled(history.canUndo());
	    mRedoButton.setEnabled(history.canRedo());
	}
    }

    /**
     * Adds or removes Colors clicked on the CameraView image
     * 
     */
    @Override
    public void colorClicked(ColorClickEvent e) {
	if (e != null) {
	    if (e.isLeft()) {
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
			.getInstance().getService(
				ColorCalibrationService.SERVICENAME);
		MarkedColorsChangedEvent event = new MarkedColorsChangedEvent(
			this, MarkedColorsChangedEvent.MChangeType.Added,
			mSelectedColor, e.getColors(), e.getStencilSize());
		colorService.notifyMarkedColorsChange(event);
	    } else {
		ColorCalibrationService colorService = (ColorCalibrationService) ServiceManager
			.getInstance().getService(
				ColorCalibrationService.SERVICENAME);
		MarkedColorsChangedEvent event = new MarkedColorsChangedEvent(
			this, MarkedColorsChangedEvent.MChangeType.Removed,
			mSelectedColor, e.getColors(), e.getStencilSize());
		colorService.notifyMarkedColorsChange(event);
	    }
	}
	UndoRedoHistory<LookUpTable> history = mColorService.getLookUpTable()
		.getUndoRedoHistory();
	mUndoButton.setEnabled(history.canUndo());
	mRedoButton.setEnabled(history.canRedo());
    }
}
