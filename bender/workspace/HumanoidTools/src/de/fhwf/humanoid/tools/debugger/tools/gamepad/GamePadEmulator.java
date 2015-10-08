package de.fhwf.humanoid.tools.debugger.tools.gamepad;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;

public class GamePadEmulator extends JFrame implements KeyListener {
	private static final long serialVersionUID = 1L;
	
	private GamePad mParent;
	
	private int mX = 0;
	private int mY = 0;
	private int mZ = 0;
	private boolean mButtons[] = new boolean[3];
	
	public GamePadEmulator(GamePad parent) {
		super("Keyboard Gamepad");
	  this.setSize(100, 100);
	  this.setLayout(null);
	  this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
	  this.addKeyListener(this);
	  this.setVisible(true);	  
	 	mParent = parent;
	}
	
	@Override
	public void keyPressed(KeyEvent arg0) {
		boolean updated = false;
		
		if (arg0.getKeyCode() == KeyEvent.VK_UP) {
			mY = -1;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_DOWN) {
			mY = 1;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_LEFT) {
			mX = -1;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_RIGHT) {
			mX = 1;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_ALT) {
			arg0.consume();
			mButtons[0] = true;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_SPACE) {
			mButtons[1] = true;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_CONTROL) {
			mButtons[2] = true;
			updated = true;
		}
		
		if (updated) {
			mParent.setPadState(mX, mY, mZ, mButtons);
		}
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		boolean updated = false;
		
		if (arg0.getKeyCode() == KeyEvent.VK_UP) {
			if (mY == -1) {
				mY = 0;
				updated = true;
			}
		}
		if (arg0.getKeyCode() == KeyEvent.VK_DOWN) {
			if (mY == 1) {
				mY = 0;
				updated = true;
			}
		}
		if (arg0.getKeyCode() == KeyEvent.VK_LEFT) {
			if (mX == -1) {
				mX = 0;
				updated = true;
			}
		}
		if (arg0.getKeyCode() == KeyEvent.VK_RIGHT) {
			if (mX == 1) {
				mX = 0;
				updated = true;
			}
		}
		if (arg0.getKeyCode() == KeyEvent.VK_ALT) {
			arg0.consume();
			mButtons[0] = false;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_SPACE) {
			mButtons[1] = false;
			updated = true;
		}
		if (arg0.getKeyCode() == KeyEvent.VK_CONTROL) {
			mButtons[2] = false;
			updated = true;
		}

		if (updated) {
			mParent.setPadState(mX, mY, mZ, mButtons);
		}
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
	}

	public int getButtonCount() {
		return mButtons.length;
	}
}
