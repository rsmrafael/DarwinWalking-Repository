package de.fhwf.humanoid.tools.debugger.views;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JPanel;

import org.jzy3d.chart.Chart;
import org.jzy3d.chart.controllers.ControllerType;
import org.jzy3d.chart.controllers.mouse.ChartMouseController;
import org.jzy3d.chart.controllers.thread.ChartThreadController;
import org.jzy3d.colors.Color;
import org.jzy3d.events.ControllerEvent;
import org.jzy3d.events.ControllerEventListener;
import org.jzy3d.maths.Coord3d;
import org.jzy3d.plot3d.primitives.Scatter;
import org.jzy3d.plot3d.rendering.view.Renderer2d;

import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.data.YUVColor;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateEvent;
import de.fhwf.humanoid.tools.debugger.events.ColorConfigurationUpdateListener;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService.ColorID;
import de.fhwf.humanoid.tools.debugger.services.ImageService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;

/**
 * This class enables 3D ColorCalibrationVisualisation Um Programm auszuführen,
 * müssen sich Platform specific native libraries in den Java/bin ordner Kopiert
 * werden. Zu finden sind diese unter http://www.jzy3d.org/download-0.9.php
 * 
 * @author Dominik Molitor Last change 03.02.2012
 */
public class ColorCalibrationVisualisation extends JPanel implements
		ColorConfigurationUpdateListener {

	/**
     * 
     */
	private static final long serialVersionUID = 1L;
	private ColorCalibrationService mColorService;
	private Chart chart;
	private ChartMouseController mouseMotion;
	private static boolean update = false;

	public static void setUpdate(boolean updateded) {
		update = updateded;
		// System.out.println(update);
	}

	public ColorCalibrationVisualisation() {
		super();
		mColorService = (ColorCalibrationService) ServiceManager.getInstance()
				.getService(ColorCalibrationService.SERVICENAME);

		((ImageService) ServiceManager.getInstance().getService(
				ImageService.SERVICENAME))
				.addColorConfigurationUpdateListener(this);
		if (update == true) {
			createSceneGraph();
		}
	}

	@Override
	public void paintComponent(Graphics g) {
		if (update == true) {
			// System.out.println("Update = true");
			this.setIgnoreRepaint(false);
			super.paintComponent(g);
		}

		if (update == false) {
			// System.out.println("Update = false");
			try {
				this.remove((Component) chart.getCanvas());
			} catch (java.lang.NullPointerException e) {

			}
			this.setIgnoreRepaint(true);
		}
	}

	/**
	 * Repaint Aufruf passt Größe der Darstellung an JPanel an.
	 * 
	 */
	@Override
	public void repaint() {
		if (update == true) {
			// System.out.println("Update = true");
			this.setIgnoreRepaint(false);
			super.repaint();
			((Component) chart.getCanvas()).setSize(this.getWidth(),
					this.getHeight());
		}
		if (update == false) {
			// System.out.println("Update = false");
			try {
				this.remove((Component) chart.getCanvas());
			} catch (java.lang.NullPointerException e) {

			}
			this.setIgnoreRepaint(true);
		}

	}

	/**
	 * Aufruf erzeugt neue Darstellung mit Farben im Koordinatensystem
	 * 
	 */
	@Override
	public void colorConfigurationUpdated(ColorConfigurationUpdateEvent e) {

		if (update == false) {

			try {
				this.remove((Component) chart.getCanvas());
			} catch (java.lang.NullPointerException en) {

			}
			if (chart != null) {
				chart.getCanvas().dispose();

			}

			chart = null;

		}

		if (update == true) {
			try {
				this.remove((Component) chart.getCanvas());
			} catch (java.lang.NullPointerException en) {

			}
			if (chart != null) {
				chart.getCanvas().dispose();

			}

			chart = null;
			chart = new Chart();
			chart.addRenderer(new Renderer2d() {
				public void paint(Graphics g) {

				}
			});
			createSceneGraph();
			// System.out.println("Updated");
		}

	}

	public void createSceneGraph() {

		// update = true;
		// Adden der Punkte im Koordinatensystem

		Coord3d[][][] points = new Coord3d[64][64][64];
		Color[][][] colors = new Color[64][64][64];
		int i = 0;

		LookUpTable mTable = mColorService.getLookUpTable();
		if (mTable != null) {
			for (int y = 0; y < 256; y = y + 4) {
				for (int u = 0; u < 256; u = u + 4) {
					for (int v = 0; v < 256; v = v + 4) {
						if (mTable.isColor(ColorID.Orange,
								new YUVColor(y, u, v))) {
							// System.out.println("Orange");
							points[y / 4][u / 4][v / 4] = new Coord3d(y, u, v);
							colors[y / 4][u / 4][v / 4] = new Color(205, 133, 0);
							i++;

						} else if (mTable.isColor(ColorID.Yellow, new YUVColor(
								y, u, v))) {
							// System.out.println("Yellow");
							points[y / 4][u / 4][v / 4] = new Coord3d(y, u, v);
							colors[y / 4][u / 4][v / 4] = Color.YELLOW;
							i++;
						}

						else if (mTable.isColor(ColorID.Blue, new YUVColor(y,
								u, v))) {
							// System.out.println("Blue");
							points[y / 4][u / 4][v / 4] = new Coord3d(y, u, v);
							colors[y / 4][u / 4][v / 4] = Color.BLUE;
							i++;

						}

						else if (mTable.isColor(ColorID.Green, new YUVColor(y,
								u, v))) {
							// System.out.println("Green");
							points[y / 4][u / 4][v / 4] = new Coord3d(y, u, v);
							colors[y / 4][u / 4][v / 4] = Color.GREEN;
							i++;

						}

					}
				}
			}

		}

		// 3D auf 1D Array umschreiben
		Coord3d[] pointsAll = new Coord3d[i + 3];
		Color[] colorsAll = new Color[i + 3];
		//System.out.println("Anzahl Punkte:" + i);

		pointsAll[0] = new Coord3d(0, 0, 256);
		pointsAll[1] = new Coord3d(0, 256, 0);
		pointsAll[2] = new Coord3d(256, 0, 0);
		colorsAll[0] = Color.BLUE;
		colorsAll[1] = Color.BLUE;
		colorsAll[2] = Color.BLUE;

		i = 3;
		for (int y = 0; y < 64; y = y + 1) {
			for (int u = 0; u < 64; u = u + 1) {
				for (int v = 0; v < 64; v = v + 1) {
					if (points[y][u][v] != null) {
						pointsAll[i] = points[y][u][v];
						colorsAll[i] = colors[y][u][v];
						i++;
					}

				}
			}
		}
		points = null;
		colors = null;

		// Adden der Punkte in ein Scatter
		Scatter scatter = new Scatter(pointsAll, colorsAll, 6);
		// Scatter scatterBounds = new Scatter(boundsCords, boundsColor);

		// Erzeugen der Chart

		// Mouse Movement erzeugen
		mouseMotion = new ChartMouseController();
		ChartThreadController thread = new ChartThreadController();
		mouseMotion.addSlaveThreadController(thread);
		mouseMotion.addControllerEventListener(new ControllerEventListener() {
			public void controllerEventFired(ControllerEvent e) {
				if (e.getType() == ControllerType.ROTATE) {
				}
			}
		});

		// Adden Movement und Scatter zur Chart
		chart.addController(mouseMotion);
		chart.addController(thread);
		chart.getScene().add(scatter);
		// chart.getScene().add(scatterBounds);

		// Panel einrichten und adden der Chart
		GridBagLayout gl = new GridBagLayout();
		GridBagConstraints gbc = new GridBagConstraints();

		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.gridwidth = 0;
		gbc.gridheight = 0;
		this.setLayout(gl);
		setVisible(true);
		((Component) chart.getCanvas()).setSize(this.getWidth(),
				this.getHeight());

		this.add((Component) chart.getCanvas(), gbc);

	}

}