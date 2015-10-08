package de.fhwf.humanoid.tools.debugger.tools;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.border.EtchedBorder;

import de.fhwf.humanoid.tools.debugger.data.YUVColor;

public class ColorInfo extends JPanel {
	private static final long serialVersionUID = 6801292311325365147L;

	private YUVColor mCurrentColor;
	private JLabel mLblYValue;
	private JLabel mLblUValue;
	private JLabel mLblVValue;
	private JLabel mColorField;
	
	public ColorInfo() {
		mCurrentColor = new YUVColor(0, 0, 0);
			
		JPanel values = new JPanel(new GridLayout(3, 2));
		Dimension d = new Dimension(30, 10);
		values.add(new JLabel("Y:"));
		mLblYValue = new JLabel(Integer.toString(mCurrentColor.Y));
		mLblYValue.setPreferredSize(d);
		mLblYValue.setHorizontalAlignment(SwingConstants.RIGHT);
		values.add(mLblYValue);
		values.add(new JLabel("U:"));
		mLblUValue = new JLabel(Integer.toString(mCurrentColor.U));
		mLblUValue.setPreferredSize(d);
		mLblUValue.setHorizontalAlignment(SwingConstants.RIGHT);
		values.add(mLblUValue);
		values.add(new JLabel("V:"));
		mLblVValue = new JLabel(Integer.toString(mCurrentColor.V));
		mLblVValue.setPreferredSize(d);
		mLblVValue.setHorizontalAlignment(SwingConstants.RIGHT);
		values.add(mLblVValue);
		this.add(values);
		
		mColorField = new JLabel();
		d = new Dimension(50, 50);
		mColorField.setMinimumSize(d);
		mColorField.setPreferredSize(d);
		mColorField.setOpaque(true);
		mColorField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED, Color.WHITE, Color.LIGHT_GRAY));
		setColor(mCurrentColor);
		this.add(mColorField);
		this.setVisible(true);
	}
	
	public void setColor(YUVColor color) {
		mCurrentColor = color;
		mColorField.setBackground(new Color(mCurrentColor.R, mCurrentColor.G, mCurrentColor.B));
		mLblYValue.setText(Integer.toString(color.Y));
		mLblUValue.setText(Integer.toString(color.U));
		mLblVValue.setText(Integer.toString(color.V));
	}
	
	public YUVColor getColor() {
		return mCurrentColor;
	}
}
