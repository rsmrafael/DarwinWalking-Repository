package de.fhwf.humanoid.tools.debugger.events;

import java.util.EventObject;
import java.util.List;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;

public class ColorClickEvent extends EventObject {
	private static final long serialVersionUID = 1L;

	private List<YUVColor> mColors;
	private boolean imLeft;
	private int mStencilSize;
  
	/**
	 * Creates a ColorClickEvent used to get the information from the Image to the Color Calibration tool.
	 * 
	 * @param source	
	 * @param colors	List of Colors that shall be marked
	 * @param isLeft	if the click is a left click.
	 * @param mStencilSize the Radius of the 3d Stencil used to mark colors in the space.
	 */
  public ColorClickEvent(Object source, List<YUVColor> colors, boolean isLeft, int mStencilSize) { 
    super(source); 
    mColors = colors; 
    imLeft = isLeft;
    this.mStencilSize = mStencilSize;
  } 
 
  /**
   * Returns the List of Colors.
   * 
   * @return
   */
  public List<YUVColor> getColors() { 
    return mColors;
  } 
  
  /**
   * The Color click Event is used for both adding and subtracting color from the selection. 
   * Therefore the left and right clicks are separated.
   * 
   * @return true if the Click was Left, false if it was right.
   */
  public boolean isLeft(){
	  return imLeft;
  }
  
  /**
   * The StencilSize for Marking the Colors in the YUV Space
   * @return
   */
  public int getStencilSize(){
	  return mStencilSize;
  }
}