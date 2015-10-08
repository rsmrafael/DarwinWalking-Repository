package de.fhwf.humanoid.tools.debugger.blackboard.representations;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

import javax.swing.JComponent;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.data.YUVImage;
import de.fhwf.humanoid.tools.debugger.gui.BlackBoardFrame;
import de.fhwf.humanoid.tools.debugger.tools.ColorInfo;
import de.fhwf.humanoid.tools.debugger.visualizer.ImageVisualizer;

public class YUVImageRepresentation extends IRepresentation {

	private YUVImage mImage;

	public YUVImageRepresentation(ByteString data)
			throws InvalidProtocolBufferException {
		de.ostfalia.robocup.message.MsgRepresentations.YUVImage imgMsg = de.ostfalia.robocup.message.MsgRepresentations.YUVImage
				.parseFrom(data);
		mImage = new YUVImage(imgMsg.getData().toByteArray(),
				imgMsg.getWidth(), imgMsg.getHeight());

		registerVisualizer("Image");
	}

	@Override
	public void serialize() {
	}

	public BufferedImage getBufferedImage() {
		return mImage.getImage();
	}

	public BufferedImage getYChannel() {
		return mImage.getYImage();
	}

	public BufferedImage getUChannel() {
		return mImage.getUImage();
	}

	public BufferedImage getVChannel() {
		return mImage.getVImage();
	}

	public int getWidth() {
		return mImage.getWidth();
	}

	public int getHeigth() {
		return mImage.getHeigth();
	}
	
	public YUVColor getColor(int x, int y){
		return mImage.getImageData(x, y);
	}

	@Override
	public void showEditor(String visualizer) {
		if (visualizer.equals("Image")) {
			ColorInfo cf = new ColorInfo();
			
			ArrayList<JComponent> list = new ArrayList<JComponent>();
			
			list.add(new ImageVisualizer(this, cf));
			list.add(cf);
			
			new BlackBoardFrame(list, "YuvImage").setSize(new Dimension(850,400));
		} else {
			System.out
					.println("[Point Cloud Representation] - Visualizer not Found");
		}

	}
}
