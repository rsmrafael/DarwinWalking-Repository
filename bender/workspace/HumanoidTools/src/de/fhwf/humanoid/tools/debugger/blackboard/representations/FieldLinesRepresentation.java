package de.fhwf.humanoid.tools.debugger.blackboard.representations;

import java.util.ArrayList;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;

import de.fhwf.humanoid.tools.debugger.data.DrawObject;
import de.fhwf.humanoid.tools.debugger.data.DrawObject.DrawColor;
import de.fhwf.humanoid.tools.debugger.data.DrawObject.DrawShape;
import de.fhwf.humanoid.tools.debugger.events.DrawObjectsEvent;
import de.fhwf.humanoid.tools.debugger.gui.BlackBoardFrame;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.services.VisionDataService;
import de.fhwf.humanoid.tools.debugger.visualizer.ListVisualizer;
import de.ostfalia.robocup.message.MsgFieldLines.FieldLines;
import de.ostfalia.robocup.message.MsgLine.Line;

public class FieldLinesRepresentation extends IRepresentation {

	private ArrayList<LineRepresentation> mLines;

	public FieldLinesRepresentation(ByteString data)
			throws InvalidProtocolBufferException {

		FieldLines fieldLines = de.ostfalia.robocup.message.MsgFieldLines.FieldLines
				.parseFrom(data);

		mLines = new ArrayList<LineRepresentation>();

		for (int i = 0; i < fieldLines.getLineCount(); ++i) {
			Line protoLine = fieldLines.getLine(i);
			LineRepresentation line = new LineRepresentation();
			line.mStartX = protoLine.getStartPoint().getX();
			line.mStartY = protoLine.getStartPoint().getY();
			line.mEndX = protoLine.getEndPoint().getX();
			line.mEndY = protoLine.getEndPoint().getY();

			mLines.add(line);
		}

		registerVisualizer("List");
		registerVisualizer("Image Overlay");
	}

	@Override
	public void serialize() {
		// TODO Auto-generated method stub

	}

	public ArrayList<LineRepresentation> getCoud() {
		return mLines;
	}

	public int getSize() {
		return mLines.size();
	}

	@Override
	public void showEditor(String visualizer) {

		if (visualizer.equals("List")) {
			new BlackBoardFrame(new ListVisualizer(this), "FieldLines");
		} else if (visualizer.equals("Image Overlay")) {
			VisionDataService mVisionDataService = (VisionDataService) ServiceManager
					.getInstance().getService(VisionDataService.SERVICENAME);

			ArrayList<DrawObject> objects = new ArrayList<DrawObject>();

			for (LineRepresentation p : mLines) {
				objects.add(new DrawObject(mSource, DrawShape.LINE,
						DrawColor.RED, p.mStartX, p.mStartY, p.mEndX, p.mEndY));
			}

			DrawObjectsEvent event = new DrawObjectsEvent(this, objects);
			mVisionDataService.notifyNewDrawObjects(event);
		} else {
			System.out
					.println("[Point Cloud Representation] - Visualizer not Found");
		}

	}

}
