package de.fhwf.humanoid.tools.debugger.undoredo;

import java.util.Stack;

public class UndoRedoHistory<T> {
	private boolean mInUndoRedo = false;
	private T mSubject;
	
  private Stack<IMemento<T>> mUndoStack;
  private Stack<IMemento<T>> mRedoStack;
  
  private CompoundMemento<T> tempMemento = null;

  public UndoRedoHistory(T subject) {
  	mSubject = subject;
  	mUndoStack = new Stack<IMemento<T>>();
  	mRedoStack = new Stack<IMemento<T>>();
  }
  
  public boolean isInUndoRedo() {
    return mInUndoRedo;
  }

  public int getUndoCount() {
    return mUndoStack.size();
  }
  
  public int getRedoCount() {
    return mRedoStack.size();
  }

  public void Do(IMemento<T> m) {
    if (mInUndoRedo) {
      throw new IllegalStateException("Involking do within an undo/redo action.");
    }
    
    if (tempMemento == null) {
        _Do(m);
    } else {
        tempMemento.add(m);
    }    
  }
  
  private void _Do(IMemento<T> m) {
    mRedoStack.clear();
    mUndoStack.push(m);
  }
  
  public void undo() {
    mInUndoRedo = true;
    IMemento<T> top = mUndoStack.pop();
    mRedoStack.push(top.restore(mSubject));
    mInUndoRedo = false;
  }
  
  public void redo() {
    mInUndoRedo = true;
    IMemento<T> top = mRedoStack.pop();
    mUndoStack.push(top.restore(mSubject));
    mInUndoRedo = false;
  }
  
  public boolean canUndo() {
    return (mUndoStack.size() != 0);
  }

  public boolean canRedo() {
    return (mRedoStack.size() != 0);
  }

  public void clear() {
    mUndoStack.clear();
    mRedoStack.clear();
  }

  public IMemento<T> PeekUndo() {
    if (mUndoStack.size() > 0) {
      return mUndoStack.peek();
    } else {
      return null;
    }
  }

  public IMemento<T> PeekRedo() {
    if (mRedoStack.size() > 0) {
      return mRedoStack.peek();
    } else {
      return null;
    }
  }
  
  public void beginCompoundDo() {
      if (tempMemento != null) {
          throw new IllegalStateException("Previous complex memento wasn't commited.");
      }

      tempMemento = new CompoundMemento<T>();
  }

  public void endCompoundDo() {
      if (tempMemento == null)
          throw new IllegalStateException("Ending a non-existing complex memento");

      if (tempMemento.getSize() > 0) {
      	_Do(tempMemento);
      }
      tempMemento = null;
  }
}
