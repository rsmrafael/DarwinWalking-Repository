package de.fhwf.humanoid.tools.debugger.undoredo;

import java.util.LinkedList;
import java.util.List;

public class CompoundMemento<T> implements IMemento<T> {
  private List<IMemento<T>> mMementos = new LinkedList<IMemento<T>>();
  
  public void add(IMemento<T> m) {
  	if (m instanceof StateSavingMemento) {
  		// When saving the whole state, it's enough to
  		// save the first memento
  		if (mMementos.size() == 0) {
  			mMementos.add(m);
  		}
  	} else {
  		mMementos.add(m);
  	}
  }

  public int getSize() {
    return mMementos.size();
  }
  
	@Override
  public CompoundMemento<T> restore(T target) {
      CompoundMemento<T> inverse = new CompoundMemento<T>();
      //starts from the last action
      for (int i = mMementos.size() - 1; i >= 0; i--) {
          inverse.add(mMementos.get(i).restore(target));
      }
      return inverse;
  }
}
