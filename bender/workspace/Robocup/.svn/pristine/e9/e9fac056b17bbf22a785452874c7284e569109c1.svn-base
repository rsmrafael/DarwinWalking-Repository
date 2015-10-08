public class PositionenUmrechner 
{
	private DT_INIFile ini;
	public PositionenUmrechner()
	{
		ini = new DT_INIFile("DT_INIFile\\Positionen.ini");
		
	}
	public void berechnePositionenAusIni()
	{
		int[][] szenenDaten = getScenePositons();
		szenenDaten = verarbeiteDaten(szenenDaten);
		schreibeDaten(szenenDaten);
		ini.save();
	}
	public int[][] getScenePositons()
	{
		int szenenAnzahl = ini.getAllSectionNames().length;
		int[][]ergebnis = new int[szenenAnzahl][18];
		for(int index1 =0;index1 < szenenAnzahl;index1++)
		{
			String szene = ini.getStringProperty("Motion", ""+index1);
			String[] positionen = szene.split(",");
			for(int index2 =0;index2<positionen.length;index2++)
			{
				ergebnis[index1][index2]= Integer.parseInt(positionen[index2]);
			}
			
		}
		return ergebnis;
	}
	public int[][] verarbeiteDaten(int[][] eingangsdaten)
	{
		String[] zeroFirst = ini.getStringProperty("ZeroPosition",""+1).split(",");
		String[] zeroSecond = ini.getStringProperty("ZeroPosition",""+2).split(",");
		int[][] ausgangsdaten = new int[eingangsdaten.length][eingangsdaten[0].length];
		for(int index1 =0;index1<eingangsdaten.length;index1++)
		{
			for(int index2 =0;index2<eingangsdaten[index1].length;index2++)
			{
				eingangsdaten[index1][index2]= (eingangsdaten[index1][index2]-Integer.parseInt(zeroFirst[index2]));
				ausgangsdaten[index1][index2]= Integer.parseInt(zeroSecond[index2])+ eingangsdaten[index1][index2];
			}
		}
		return ausgangsdaten;
	}
	public void schreibeDaten(int[][]szenenDaten)
	{
		for(int index1 =0;index1<szenenDaten.length;index1++)
		{
			String szene = "";
			for(int index2 =0;index2<szenenDaten[index1].length;index2++)
			{
				szene = szene +szenenDaten[index1][index2]+",";
			}
			ini.setStringProperty("Ergebnis", ""+index1, szene, null);
		}
			
	}
}
