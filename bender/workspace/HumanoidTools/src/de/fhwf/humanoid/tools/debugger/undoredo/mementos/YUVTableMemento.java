package de.fhwf.humanoid.tools.debugger.undoredo.mementos;

import de.fhwf.humanoid.tools.debugger.data.LookUpTable;
import de.fhwf.humanoid.tools.debugger.data.ColorConfig.IYUVTable;
import de.fhwf.humanoid.tools.debugger.undoredo.IMemento;
import de.fhwf.humanoid.tools.debugger.undoredo.StateSavingMemento;

public class YUVTableMemento implements IMemento<LookUpTable>, StateSavingMemento {
	
	private IYUVTable mTable;
	
	public YUVTableMemento(IYUVTable mYUV) {
		try {
			mTable = (IYUVTable) mYUV.clone();
		} catch (CloneNotSupportedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public IMemento<LookUpTable> restore(LookUpTable target) {
    IMemento<LookUpTable> inverse = new YUVTableMemento(target.getIYUVTable());

    target.setIYUVTable(mTable);
    return inverse;
  }
}
