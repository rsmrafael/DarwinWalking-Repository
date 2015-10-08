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
import de.ostfalia.robocup.message.MsgPointCloud.PointCloud;

public class PointCloudRepresentation extends IRepresentation {

	private ArrayList<PointRepresentation> mCloud;

	public PointCloudRepresentation(ByteString data)
			throws InvalidProtocolBufferException {

		PointCloud protoPointCloud = de.ostfalia.robocup.message.MsgPointCloud.PointCloud
				.parseFrom(data);

		mCloud = new ArrayList<PointRepresentation>();

		for (int i = 0; i < protoPointCloud.getPointCount(); ++i) {
			mCloud.add(new PointRepresentation(protoPointCloud.getPoint(i)));
		}

		registerVisualizer("List");
		registerVisualizer("Image Overlay");
	}

	@Override
	public void serialize() {
		// TODO Auto-generated method stub

	}

	public ArrayList<PointRepresentation> getCoud() {
		return mCloud;
	}

	public int getSize() {
		return mCloud.size();
	}

	@Override
 public void showEditor(String visualizer) {
	 
	 if (visualizer.equals("List")){
		 new BlackBoardFrame(new ListVisualizer(this), "PointCloud");
	 } else if (visualizer.equals("Image Overlay")){
		VisionDataService mVisionDataService = (VisionDataService) ServiceManager.getInstance()
					.getService(VisionDataService.SERVICENAME);
		
		ArrayList<DrawObject> objects = new ArrayList<DrawObject>();
		
		for (PointRepresentation p : mCloud){
			objects.add(new DrawObject(mSource, DrawShape.POINT, DrawColor.RED,p.x, p.y, 0, 0));
		}
		
		DrawObjectsEvent event = new DrawObjectsEvent(this, objects);
		mVisionDataService.notifyNewDrawObjects(event);
	 } else {
		 System.out.println("[Point Cloud Representation] - Visualizer not Found");
	 }
	 
  
 }

}
