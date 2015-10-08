package de.fhwf.humanoid.tools.debugger.tools;

public class Stencil {
	
	
	public static int[][][] getStencil3D(int radius){
		//TODO: schön machen ggf als Kugel
		int[][][] newStencil =  new int[2*radius+1][2*radius+1][2*radius+1];
		
		for (int x = 0; x < 2*radius+1; x++){
			for (int y = 0; y < 2*radius+1; y++){
				for (int z = 0; z < 2*radius+1; z++){
					newStencil[x][y][z] = 1;
				}
			}
		}
		return newStencil;
	}
	
	public static int[][] getStencil2D(int radius){
		//TODO: schön machen ggf als kreis
		int[][] newStencil =  new int[2*radius+1][2*radius+1];
		
		for (int x = 0; x < 2*radius+1; x++){
			for (int y = 0; y < 2*radius+1; y++){
				newStencil[x][y] = 1;
			}
		}	
		return newStencil;
	}

}
