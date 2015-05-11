/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javathreads;

import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yova
 */
public class JavaThreads {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
//        System.out.println(args.length);
//        if(args.length != 2){
//            System.out.println("Argumentos Invalidos");
//        }
        
        JavaThreads threads = new JavaThreads();
        threads.ThreadLauncher(0, 14);
//        threads.AnimConvThreads(0,3);

    }
    
    public void ThreadLauncher(int initFrame, int endFrame){
        int threadCount = 5;
        int totalFrames = endFrame - initFrame + 1;        
        int frameCount = initFrame;
        
//        System.out.println("InitFrame: "+initFrame+", EndFrame: "+endFrame+
//                ", TotalFrames: "+totalFrames);
        
        while(totalFrames > threadCount){
//            System.out.println("Launching => InitFrame: "+frameCount+", EndFrame: "+
//                    (frameCount+threadCount-1));
            AnimConvThreads(frameCount,frameCount+threadCount-1);
            frameCount += threadCount;
            totalFrames -= threadCount;
        }
//        System.out.println("Finally => InitFrame: " + frameCount+ ", EndFrame: "+
//                endFrame);
        AnimConvThreads(initFrame, endFrame);        
    }
    
    public void AnimConvThreads(int initFrame, int endFrame){
        System.out.println( "Starting Executor" );

        // create ExecutorService to manage threads
        ExecutorService threadExecutor = Executors.newCachedThreadPool();
        
        for(int i = initFrame; i <= endFrame; i++){
            AnimConv anim = new AnimConv(i);
            threadExecutor.execute( anim ); // start task1        
        }
        
        // shut down worker threads when their tasks complete
        threadExecutor.shutdown();
        while(!threadExecutor.isTerminated()){
            try {
                Thread.sleep( (long)10000 );
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaThreads.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println( "Tasks block ends...\n" );                
    }
    
}
