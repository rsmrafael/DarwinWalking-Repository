package de.fhwf.humanoid.tools.debugger.data;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import com.google.protobuf.ByteString;
import de.ostfalia.robocup.message.MsgImageData.ImageData;
import de.ostfalia.robocup.message.MsgImageData.ImageFormat;
import de.ostfalia.robocup.message.MsgProtoBufFrame.CameraLocation;
import de.ostfalia.robocup.message.MsgProtoBufFrame.ProtoBufFrame;
import de.ostfalia.robocup.message.MsgProtoBufFrame.ProtoBufFrame.Camera;

public class PBFrame {
 private ProtoBufFrame mFrame = null;
 
 public PBFrame() {
 }

 public void load(String fileName) throws FileNotFoundException, IOException {
  mFrame = ProtoBufFrame.parseFrom(new FileInputStream(fileName));
 }
 
 public YUVImage getImage(CameraLocation location) {
  if (mFrame != null) {
   for (Camera cam : mFrame.getCameraList()) {
    if (cam.getType().getLocation().equals(location)) {
     for (ImageData img : cam.getImageDataList()) {
      if (img.getFormat().equals(ImageFormat.YUV422_IMAGE)) {
       ByteString imgData = img.getData();
       int width = img.getWidth();
       int height = img.getHeight();
       YUVImage yuv = null;
       try {
        char[] data = imgData.toString("ISO-8859-1").toCharArray();
        yuv = new YUVImage(data, width, height);
       } catch (UnsupportedEncodingException e) {
       }
       return yuv;
      }
     }
    }
   }
  }
  return null;
 }
}
