/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javathreads;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yova
 */
public class AnimConv implements Runnable{
    int timeShift;

    public AnimConv(int _time) {
        timeShift = _time;
    }
    
    @Override
    public void run() {

        String[] command =
        {
            "cmd",
        };
        Process p = null;
        try {
            p = Runtime.getRuntime().exec(command);
        } catch (IOException ex) {
            Logger.getLogger(AnimConv.class.getName()).log(Level.SEVERE, null, ex);
        }
        new Thread(new SyncPipe(p.getErrorStream(), System.err)).start();
        new Thread(new SyncPipe(p.getInputStream(), System.out)).start();
        PrintWriter stdin = new PrintWriter(p.getOutputStream());
        
        Thread.currentThread().setPriority(Thread.MAX_PRIORITY);
        
//        stdin.println("cd C:/Users/yova/Documents/TesisDocs/TesisMaestria/"
//                + "nD-EVM/dist/Debug/MinGW-Windows");
        
        stdin.println("AnimConv2.exe 4 4 3 " +timeShift);

        stdin.close();
        int returnCode = 0;
        try {
            returnCode = p.waitFor();
        } catch (InterruptedException ex) {
            Logger.getLogger(AnimConv.class.getName()).log(Level.SEVERE, null, ex);
        }
        System.out.println("Return code = " + returnCode);
        
//        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
