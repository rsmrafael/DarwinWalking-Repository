import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.WindowConstants;

/**
 * 
 * @author Bombadil
 *
 */
public class errorFrame extends JDialog
{

	/**
	 * 
	 */
	private static final long serialVersionUID = -11458091801228869L;
	private JTextArea text = new JTextArea();
	private Image ima;
	public errorFrame(JFrame owner,String title, boolean modal,String fehler)
	{
		super(owner, title, modal);
	    setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
	    int frameWidth = 600;
	    int frameHeight = 120;
	    setSize(frameWidth, frameHeight);
	    Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
	    int x = (d.width - getSize().width) / 2;
	    int y = (d.height - getSize().height) / 2;
	    setLocation(x, y);
	    Container cp = getContentPane();
	    cp.setLayout(null);
	    text.setBounds(20,20,500,80);
	    text.setText(fehler);
	    text.setFocusable(false);
	    text.setBorder(null);
	    text.setBackground(cp.getBackground());
	    cp.add(text);
	    ima = Toolkit.getDefaultToolkit().getImage("Iamges\\Error.gif");
	    setResizable(false);
	    setVisible(true);
	}
	 public void paint(Graphics g)
     {
        super.paint(g);
        g.drawImage(ima,522,40, this);
     }
}
