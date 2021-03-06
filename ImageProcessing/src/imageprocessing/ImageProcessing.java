/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package imageprocessing;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yova
 */
public class ImageProcessing {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        ImageProcessing test = new ImageProcessing();
        try {
            test.imagesTest();
//            test.pasilloTest();
        } catch (IOException ex) {
            Logger.getLogger(ImageProcessing.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InterruptedException ex) {
            Logger.getLogger(ImageProcessing.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void imagesTest() throws IOException, InterruptedException{
        String[] command =
        {
            "cmd",
        };
        Process p = Runtime.getRuntime().exec(command);
        new Thread(new SyncPipe(p.getErrorStream(), System.err)).start();
        new Thread(new SyncPipe(p.getInputStream(), System.out)).start();
        PrintWriter stdin = new PrintWriter(p.getOutputStream());

        stdin.println("cd Sequences/Faces/");
        String fileName = "";
//        String filePath = "Sequences\\JackJack\\";
//        String command;
        // - QVGA (320x240)- 
        for(int i = 0; i <= 3; i++){
            fileName = i+".bmp";
//            command = "convert "+filePath+fileName+" -resize 50% -median 3 " +filePath+"frame"+fileName;
//            command = "convert ";
//            cout<<command<<endl;
//            system(command.c_str());
            stdin.println("C:\\ImageMagick\\convert.exe "+fileName+" -resize 50% "+ " frame"+fileName);
        }
        
        
        stdin.close();
        int returnCode = p.waitFor();
        System.out.println("Return code = " + returnCode);
    }

    public void pasilloTest() throws IOException, InterruptedException{
        String[] command =
        {
            "cmd",
        };
        Process p = Runtime.getRuntime().exec(command);
        new Thread(new SyncPipe(p.getErrorStream(), System.err)).start();
        new Thread(new SyncPipe(p.getInputStream(), System.out)).start();
        PrintWriter stdin = new PrintWriter(p.getOutputStream());

        stdin.println("cd Sequences/Pasillo/");
        String fileName = "";
//        String filePath = "Sequences\\JackJack\\"; 
//        String command;
        // - QVGA (320x240)- 
        // - HQVGA (160x120)
        int j = 0;
        for(int i = 820; i <= 874; i++){
            fileName = String.format("%04d",i)+".bmp";
//            command = "convert "+filePath+fileName+" -resize 50% -median 3 " +filePath+"frame"+fileName;
//            command = "convert ";
//            cout<<command<<endl;
//            system(command.c_str());
            stdin.println("C:\\ImageMagick\\convert.exe "+fileName+" -median 3 -gaussian-blur 3"+ " frame"+j+".bmp");
            j++;
        }
        
        
        stdin.close();
        int returnCode = p.waitFor();
        System.out.println("Return code = " + returnCode);
    }
    
    class SyncPipe implements Runnable
    {
        public SyncPipe(InputStream istrm, OutputStream ostrm) {
              istrm_ = istrm;
              ostrm_ = ostrm;
          }
          public void run() {
              try
              {
                  final byte[] buffer = new byte[1024];
                  for (int length = 0; (length = istrm_.read(buffer)) != -1; )
                  {
                      ostrm_.write(buffer, 0, length);
                  }
              }
              catch (Exception e)
              {
                  e.printStackTrace();
              }
          }
          private final OutputStream ostrm_;
          private final InputStream istrm_;
    }    
}
