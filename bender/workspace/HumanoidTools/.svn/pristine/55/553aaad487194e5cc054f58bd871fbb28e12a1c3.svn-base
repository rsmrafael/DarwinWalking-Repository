package de.fhwf.humanoid.tools.debugger.tools;

import java.awt.Color;
import java.awt.Component;
import java.awt.SystemColor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import java.util.List;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.SwingUtilities;
import javax.swing.event.EventListenerList;

public class CheckCombo extends JComboBox implements ActionListener {
 private static final long serialVersionUID = 4034425598099566945L;

 private List<CheckComboStore> mStores;
 private EventListenerList mListeners = new EventListenerList();
 
 public CheckCombo() {
  super();
  super.setRenderer(new CheckComboRenderer());
  super.addActionListener(this);
  
  mStores = new LinkedList<CheckComboStore>();
 }

 public List<Object> getSelectedItems() {
  List<Object> itms = new LinkedList<Object>();
  
  for (CheckComboStore s : mStores) {
   if (s.state) {
    itms.add(s.id);
   }
  }
  
  return itms;
 }

 @Override
 public void addItem(Object arg0) {
  CheckComboStore store = new CheckComboStore(arg0, false);
  mStores.add(store);
  super.addItem(store);
 }

 @Override
 public Object getSelectedItem() {
  if (super.getSelectedItem() != null) {
   return ((CheckComboStore)super.getSelectedItem()).id;
  } else {
   return null;
  }
 }

 @Override
 public Object getItemAt(int arg0) {
  if (super.getItemAt(arg0) != null) {
   return ((CheckComboStore)super.getItemAt(arg0)).id;
  } else {
   return null;
  }
 }

 @Override
 public void removeItem(Object arg0) {
  CheckComboStore ccs = null;
  for (CheckComboStore ccs2 : mStores) {
   if (ccs2.id.equals(arg0)) {
    super.removeItem(ccs2);
    ccs = ccs2;
   }
  }
  if (ccs != null) {
   mStores.remove(ccs);
  }
 };
 
 @Override
 public void actionPerformed(ActionEvent e) {
  
  if (!(e.getSource() instanceof CheckCombo)) {
   return;
  }
   
  final JComboBox cb = (JComboBox)e.getSource();
  if (super.getSelectedItem() instanceof CheckComboStore) {
   CheckComboStore store = (CheckComboStore)super.getSelectedItem();
   CheckComboRenderer ccr = (CheckComboRenderer)cb.getRenderer();
   ccr.checkBox.setSelected((store.state = !store.state));
   SwingUtilities.invokeLater(new Runnable(){
    public void run() {
       cb.showPopup();
    }
   });
   for (ActionListener l : mListeners.getListeners(ActionListener.class)) {
    l.actionPerformed(e);
   }
  }
 }
 
 @Override
 public void addActionListener(ActionListener l) {
  mListeners.add(ActionListener.class, l);
 }
 
 @Override
 public void removeActionListener(ActionListener l) {
  mListeners.remove(ActionListener.class, l);
 }
}

/** adapted from comment section of ListCellRenderer api */
class CheckComboRenderer implements ListCellRenderer {
 JCheckBox checkBox;

 public CheckComboRenderer() {
  checkBox = new JCheckBox();
 }

 public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
  CheckComboStore store = (CheckComboStore)value;
  checkBox.setText(store.id.toString());
  checkBox.setSelected(((Boolean)store.state).booleanValue());
  checkBox.setBackground(isSelected ? SystemColor.textHighlight : Color.white);
  checkBox.setForeground(isSelected ? Color.white : Color.black);
  return checkBox;
 }
}

class CheckComboStore {
 Object id;
 Boolean state;

 public CheckComboStore(Object id, Boolean state) {
  this.id = id;
  this.state = state;
 }
}