import java.awt.Color;
import java.awt.Image;
/**
  *
  * Beschreibung
  *
  * @version 1.0 vom %DATUM%
  * @author %AUTOR%
  */
  public class Rastergitter
  {
     //Variablen werden hier Initialiesiert
     private int fWidhth = 600;
     private int fHeight = 600;
     private int zellHeight = 50;
     private int zellWidth = 50;
     private int counterZell1 =0;
     public int getCounterZell1() {
		return counterZell1;
	}
	public void setCounterZell1(int counterZell1) {
		this.counterZell1 = counterZell1;
	}
	private int counterZell2 =0;
     public int getCounterZell2() {
		return counterZell2;
	}
	public void setCounterZell2(int counterZell2) {
		this.counterZell2 = counterZell2;
	}
	private Image[] imgarr = new Image[100];
     private int[] imghohe = new int[100];
     private int[] imgbreite = new int[100];
     private Color backgroundColor = Color.WHITE;
     private boolean zeigeRaster = false;
     //Geter und Seter
     public int getfWidth()
     {
        return fWidhth;
     }
     public int getfHeight()
     {
        return fHeight;

     }

     public int getZellHeight()
     {
        return  zellHeight;
     }
     public int getZellWidth()
     {
        return  zellWidth;
     }
     public Color getBackgroundColor()
     {
        return  backgroundColor;
     }
     public boolean getZeigeRaster()
     {
        return  zeigeRaster;
     }
     public Image[] getimgarr()
     {
        return imgarr;
     }
     public Image getImage(int a)
     {
    	 return imgarr[a];
     }
     public Image getimgarr(int i)
     {
          return imgarr[i];
     }
     public int getimghohe(int i)
     {
          return imghohe[i];
     }
     public int getimgbreite(int i)
     {
          return imgbreite[i];
     }
     public void setfWidth(int wit)
     {
        fWidhth = wit;
     }
     public void setfHeight(int hei)
     {
        fHeight = hei;
     }
     public void setZellHeight(int hei)
     {
        zellHeight = hei;
     }
       public void setZellWidth(int wit)
     {
        zellWidth = wit;
     }
     public void setBackgroundColor(Color c)
     {
       backgroundColor = c;
     }
     public void setZeigeRaster(boolean ok)
     {
        zeigeRaster = ok;
     }

      //Methoden
      public void initCounter()
      {
         int zellw =  zellWidth;
         int zellh =  zellHeight;
         while(zellw < fWidhth)
         {
           zellw = zellw+ zellWidth;
           counterZell1++;
         }
         while(zellh < fHeight)
         {
            zellh = zellh+ zellHeight;
            counterZell2++;
         }
         for(int i = 0; i < imghohe.length;i++)
         {
        	 imghohe[i]= Integer.MAX_VALUE;
         }
         for(int i = 0; i < imgbreite.length;i++)
         {
        	 imgbreite[i]= Integer.MAX_VALUE;
         }
      }
      @SuppressWarnings("unused")
	private int anzahlFelder()
      {
         return ((counterZell1+1)*(counterZell2+1));
      }
      public void drawImaget(Image img,int Feldx,int Feldy)
      {
        img = scale(img);
        imgarr[nextFree(imgarr)]=img;
        imghohe[nextFree(imghohe)]= Feldy*zellHeight;
        imgbreite[nextFree(imgbreite)]=Feldx*zellWidth;
      }
      public void replaceImage(int nummer,int Feldx,int Feldy)
      {

               imghohe[nummer] = Feldy*zellHeight;
               imgbreite[nummer] =Feldx*zellWidth;
      }
      public void deleteImage(int Feldx, int Feldy)
      {
    	  if(!isFeldFree(Feldx,Feldy))
    			  {
    		  		for(int i=0; i< imgbreite.length;i++)
    		  		{
    		  			if(imgbreite[i]/zellWidth==Feldx)
    		  			{
    		  				for(int i2 =0;i2<imgbreite.length;i2++)
    		  				{
    		  					if(imghohe[i2]/zellHeight==Feldy)
    		  					{
    		  						imgarr[i2]=null;
    		  						imgbreite[i2] = -1;
    		  						imghohe[i2]= -1;
    		  					}
    		  				}
    		  			}
    		  		}
    			  }
      }
      public void deleteImage(int nummer)
      {
    	
		for(int i= nummer;i<imgbreite.length-1;i++)
		{
			imgarr[i]=imgarr[i+1];
			imgbreite[i] = imgbreite[i+1];
			imghohe[i]= imghohe[i+1];
		}
      }
      public void clearRaster()
      {
    	for(int i=0; i<imgbreite.length;i++)
    	{
    		for(int i2 =0;i2<imgbreite.length;i2++)
    		{
    			deleteImage(i,i2);
    		}
    	}
      }
      public boolean isFeldFree(int Feldx, int Feldy)
      {
        boolean oki = true;
        for(int i = 0; i< nextFree(imgarr);i++)
        {
            if((Feldx*zellWidth)==imgbreite[i]&& (Feldy*zellHeight) ==imghohe[i])
            {
               oki = false;
            }
        }
        return oki;
      }
      public int nummerImage(int Feldx, int Feldy)
      {
    	  int ergebnis = -1;
    	  for(int i =0; i < imghohe.length;i++)
    	  {
    		  if(imghohe[i]/zellHeight == Feldy)
    		  {
    			  
    			  for(int i2 =0; i2 < imgbreite.length;i2++)
    			  {
    				  if(imgbreite[i2]/zellWidth == Feldx && i == i2)
    				  {
    					  ergebnis = i;
    				  }
    			  }
    		  }
    	  }
    	  return ergebnis;
      }
      public int nummerImage(Image img)
      {
    	  int ergebnis = -1;
    	  for(int i=0; i< imgarr.length;i++)
    	  {
    		  if(img.equals(imgarr[i]))
    		  {
    			  ergebnis = i;
    		  }
    	  }
    	  return ergebnis;
      }
      public int nextFree(int[] arr)
      {
         int i2 =0;
         int counter =0;
         for(int i =0;i< arr.length && i2==0  ;i++)
         {
                 if(arr[i] == Integer.MAX_VALUE && counter ==0)
                 {
                    i2= i;
                    counter++;
                 }
         }
         
         return i2;
      }
      public int nextFree(Image[] arr)
      {
      
         int i2 =0;
         int counter =0;
         for(int i =0;i< arr.length && i2==0;i++)
         {
                 if(arr[i] == null&& counter ==0)
                 {
                    i2= i;
                    counter++;
                 }
         }
         return i2;
      }
      public Image scale(Image img)
      {
          img = img.getScaledInstance(zellWidth,zellHeight,Image.SCALE_DEFAULT);
          return img;
      }



  }