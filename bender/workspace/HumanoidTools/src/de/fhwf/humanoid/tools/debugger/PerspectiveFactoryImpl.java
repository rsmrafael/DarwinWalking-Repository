package de.fhwf.humanoid.tools.debugger;

import javax.swing.JMenu;

import org.flexdock.docking.DockingConstants;
import org.flexdock.perspective.LayoutSequence;
import org.flexdock.perspective.Perspective;
import org.flexdock.perspective.PerspectiveFactory;
import org.flexdock.perspective.actions.OpenPerspectiveAction;

public class PerspectiveFactoryImpl implements PerspectiveFactory {
	public static final String DEFAULT_PERSPECTIVE_ID = "debugger_std";
	public static final String SECONDARY_PERSPECTIVE_ID = "debugger_sec";
	public static final String VISION_PERSPECTIVE_ID = "debugger_vision";
	
	@Override
	public Perspective getPerspective(String persistentId) {
		Perspective perspective = null;
		if (DEFAULT_PERSPECTIVE_ID.equals(persistentId)) {
			perspective = new Perspective(DEFAULT_PERSPECTIVE_ID, "Default");
			LayoutSequence seq = perspective.getInitialSequence(true);

			seq.add(ViewFactory.LOG);
			seq.add(ViewFactory.CAMERA, ViewFactory.LOG, DockingConstants.EAST_REGION, 0.65f);
		  seq.add(ViewFactory.BODYCAL, ViewFactory.CAMERA, DockingConstants.SOUTH_REGION, 0.5f);
		  seq.add(ViewFactory.COLORCAL, ViewFactory.BODYCAL, DockingConstants.CENTER_REGION, 0.5f);
		} else if (SECONDARY_PERSPECTIVE_ID.equals(persistentId)) {
				perspective = new Perspective(SECONDARY_PERSPECTIVE_ID, "Secondary");
				LayoutSequence seq = perspective.getInitialSequence(true);

			seq.add(ViewFactory.LOG);
			seq.add(ViewFactory.CAMERA, ViewFactory.LOG, DockingConstants.EAST_REGION, 0.65f);
			seq.add(ViewFactory.BODYCAL, ViewFactory.LOG, DockingConstants.SOUTH_REGION, 0.5f);
			seq.add(ViewFactory.COLORCAL, ViewFactory.BODYCAL, DockingConstants.CENTER_REGION, 0.5f);
			
			
		} else if (VISION_PERSPECTIVE_ID.equals(persistentId)){
			perspective = new Perspective(VISION_PERSPECTIVE_ID, "Vision");
			LayoutSequence seq = perspective.getInitialSequence(true);

			seq.add(ViewFactory.LOG);
			seq.add(ViewFactory.CAMERA, ViewFactory.LOG, DockingConstants.WEST_REGION, 0.65f);
			seq.add(ViewFactory.VISIONOBJECTS, ViewFactory.LOG, DockingConstants.NORTH_REGION, 0.5f);
			seq.add(ViewFactory.MOVEMENTCONTROL, ViewFactory.LOG, DockingConstants.SOUTH_REGION, 0.5f);
			seq.add(ViewFactory.COLORCAL, ViewFactory.LOG, DockingConstants.SOUTH_REGION, 0.5f);
		}

		return perspective;
	}
	
	static public JMenu createPerspectivesMenu() {
		JMenu menu = new JMenu("Perspectives");
		
		menu.add(new OpenPerspectiveAction(PerspectiveFactoryImpl.DEFAULT_PERSPECTIVE_ID));
		menu.add(new OpenPerspectiveAction(PerspectiveFactoryImpl.SECONDARY_PERSPECTIVE_ID));
		menu.add(new OpenPerspectiveAction(PerspectiveFactoryImpl.VISION_PERSPECTIVE_ID));
		return menu;
	}

}
