package de.fhwf.humanoid.tools.debugger.services;

import java.util.Collection;
import java.util.HashMap;

public class ServiceManager {
	 private static ServiceManager INSTANCE;

	 private HashMap<String, IService> mStorage;
	 
	 // Not instantiable
	 private ServiceManager() {
		 mStorage = new HashMap<String, IService>();
	 }

	 public static synchronized ServiceManager getInstance() {
		  if (ServiceManager.INSTANCE == null) {
		   ServiceManager.INSTANCE = new ServiceManager();
		  }
		  return ServiceManager.INSTANCE;
	 }
	 
	 public void addService(IService service) {
		 String serviceName = service.getServiceName();
		 mStorage.put(serviceName, service);
	 }
	 
	 public Collection<IService> getServices() {
		 return mStorage.values();
	 }
	 
	 public IService getService(String serviceName) {
		 if (mStorage.containsKey(serviceName)) {
			 return mStorage.get(serviceName);
		 }
		 return null;
	 }	 
}
