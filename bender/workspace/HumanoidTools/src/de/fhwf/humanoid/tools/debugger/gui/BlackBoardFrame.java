package de.fhwf.humanoid.tools.debugger.gui;

import java.awt.BorderLayout;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JComponent;

public class BlackBoardFrame extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 8215200066261790868L;

	public BlackBoardFrame(JComponent comp, String name) {
		setTitle(name);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

		setVisible(true);
		add(comp);
		pack();
	}

	public BlackBoardFrame(ArrayList<JComponent> comps, String name) {
		setTitle(name);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		this.setLayout(new BorderLayout());

		for (JComponent comp : comps) {
			add(comp);
		}

		setVisible(true);
		pack();
	}

}
