/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javathreads;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yova
 */
public class JavaThreads {
    int frameIdx;
    int threadCount = 4;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here        
        JavaThreads threads = new JavaThreads();
//        threads.AnimConvThreadLauncher();
//        threads.ClusterContentTreadLauncher();
        threads.ClusterFrameLauncher();
//        threads.SOMClustering();
        


//        JavaThreads threads = new JavaThreads();
//        threads.AnimConvThreadLauncher(initFrame, endFrame);

    }
    
    public void SOMClustering(){
        String cmd = "";
        // create ExecutorService to manage threads
        ExecutorService threadExecutor = Executors.newCachedThreadPool();
        
        cmd = "Clustering.exe";
        CommandRunner clustering = new CommandRunner(cmd);
        threadExecutor.execute( clustering ); // start task1        
        
        // shut down worker threads when their tasks complete
        threadExecutor.shutdown();
        while(!threadExecutor.isTerminated()){
            try {
                Thread.sleep( (long)10000 );
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaThreads.class.getName()).log(Level.SEVERE, null, ex);
            }
        }        

        
        ExecutorService threadExecutor2 = Executors.newCachedThreadPool();
        
        cmd = "java -jar ClusterContentLauncher.jar";
        CommandRunner clusterContent = new CommandRunner(cmd);
        threadExecutor2.execute( clusterContent ); // start task1        
        
        // shut down worker threads when their tasks complete
        threadExecutor2.shutdown();
        while(!threadExecutor2.isTerminated()){
            try {
                Thread.sleep( (long)10000 );
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaThreads.class.getName()).log(Level.SEVERE, null, ex);
            }
        }        
    
        
        ExecutorService threadExecutor3 = Executors.newCachedThreadPool();
        
        cmd = "java -jar ClusterFrameLauncher.jar";
        CommandRunner clusterFrames = new CommandRunner(cmd);
        threadExecutor3.execute( clusterFrames ); // start task1        
        
        // shut down worker threads when their tasks complete
        threadExecutor3.shutdown();
        while(!threadExecutor3.isTerminated()){
            try {
                Thread.sleep( (long)10000 );
            } catch (InterruptedException ex) {
                Logger.getLogger(JavaThreads.class.getName()).log(Level.SEVERE, null, ex);
            }
        }        
    }
    
    public void AnimConvThreadLauncher(){
        int initFrame = 0, endFrame = 0, maskTimeLength = 0;
        // -- Lectura del archivo de configuracion...
        String fileName = "config.txt";
        String line = "";
        Scanner input = null;
        try{
            input = new Scanner( new File( fileName ) );
        } // end try
        catch ( FileNotFoundException fileNotFoundException ){
            System.err.println( "Error opening file: "+fileName );
            System.exit( 1 );
        } // end catch       
        
        // -- Se explora el contenido del archivo de configuracion linea por linea
        while(input.hasNext()){
            line = input.nextLine();
            // -- Se obtiene la configuracion de los frames
            if(line.contains("#Frames")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("initFrame:")){
                        initFrame = Integer.parseInt(words[i+1]);
                    }
                    if(words[i].equals("endFrame:")){
                        endFrame = Integer.parseInt(words[i+1]);
                    }
                }
            }
            // -- Se obtiene la longitud de la mascara para determinar el desplazamiento
            // -- maximo que se alcanza en la convolucion
            if(line.contains("#Mask")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("timeLength:")){
                        maskTimeLength = Integer.parseInt(words[i+1]);
                    }
                }
            }
            // -- Cantidad de Threads, depende de las capacidades de la PC.
            // -- Generalmente, para frames de 320x240 se usan 3 o 2, para frames de 240x160 se usan 4
            if(line.contains("#ThreadCount")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("conv:")){ // - Para convolucion
                        threadCount = Integer.parseInt(words[i+1]);
                    }
                }
            }
        }
        input.close();
        
        
        // -- se considera el maximo desplazamiento en el tiempo segun el tamano de la 
        // -- mascara
        endFrame = endFrame - maskTimeLength +1; 
        System.out.println("*** [JAVA => AnimConvThreadLauncher] initFrame: "+initFrame+", endFrame: "+endFrame+", threadCount: "+threadCount);
        
        int totalFrames = endFrame - initFrame + 1;        
        int frameCount = initFrame;
        
        // -- Se manda a ejecutar el Script AnimConv de C++ segun la configuracion de Threads
        while(totalFrames > threadCount){
            AnimConvThread(frameCount,frameCount+threadCount-1);
            frameCount += threadCount;
            totalFrames -= threadCount;
        }
        AnimConvThread(frameCount, endFrame);        
    }
    
    public void AnimConvThread(int initFrame, int endFrame){
        String cmd = "";
        // create ExecutorService to manage threads
        ExecutorService threadExecutor = Executors.newCachedThreadPool();
        
        for(int timeShift = initFrame; timeShift <= endFrame; timeShift++){
//            cmd = "AnimConv.exe 3 3 3 " +i;
            cmd = "AnimConv.exe "+timeShift;
            CommandRunner anim = new CommandRunner(cmd);
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
    }
    
    public void ClusterContentTreadLauncher(){
        int initCluster = 0, endCluster = 0;
        // -- Lectura del archivo de configuracion...
        String fileName = "config.txt";
        String line = "";
        Scanner input = null;
        try{
            input = new Scanner( new File( fileName ) );
        } // end try
        catch ( FileNotFoundException fileNotFoundException ){
            System.err.println( "Error opening file: "+fileName );
            System.exit( 1 );
        } // end catch       
        
        // -- Se explora el contenido del archivo de configuracion linea por linea
        while(input.hasNext()){
            line = input.nextLine();
            // -- Se obtiene el total de clusters
            if(line.contains("#Clustering")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("clusters:")){
                        endCluster = Integer.parseInt(words[i+1]) - 1;
                    }
                }
            }
            // -- Cantidad de Threads, depende de las capacidades de la PC.
            // -- Generalmente, para frames de 320x240 se usan 3 o 2, para frames de 240x160 se usan 4
            if(line.contains("#ThreadCount")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("clusters:")){ // - Para convolucion
                        threadCount = Integer.parseInt(words[i+1]);
                    }
                }
            }
        }
        input.close();        
        
        
        int totalClusters = endCluster + 1;        
        int clusterCount = initCluster;
        System.out.println("*** [JAVA => ClusterContentTreadLauncher] Clusters: "+totalClusters+", threadCount: "+threadCount);
        
//        System.out.println("InitFrame: "+initFrame+", EndFrame: "+endFrame+
//                ", TotalFrames: "+totalFrames);
        
        while(totalClusters > threadCount){
//            System.out.println("Launching => InitFrame: "+frameCount+", EndFrame: "+
//                    (frameCount+threadCount-1));
            ClusterContentTread(clusterCount,clusterCount+threadCount-1);
            clusterCount += threadCount;
            totalClusters -= threadCount;
        }
//        System.out.println("Finally => InitFrame: " + frameCount+ ", EndFrame: "+
//                endFrame);
        ClusterContentTread(clusterCount, endCluster);        
    }
    
    public void ClusterContentTread(int initCluster, int endCluster){
//        System.out.println( "Starting Executor" );
        String cmd = "";
        // create ExecutorService to manage threads
        ExecutorService threadExecutor = Executors.newCachedThreadPool();
        
        for(int i = initCluster; i <= endCluster; i++){
            cmd = "ClusterContent.exe "+i;
            CommandRunner anim = new CommandRunner(cmd);
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
//        System.out.println( "Tasks block ends...\n" ); 
    }
    
    public void ClusterFrameLauncher(){
        int initCluster = 0, endCluster = 0;
        // -- Lectura del archivo de configuracion...
        String fileName = "config.txt";
        String line = "";
        Scanner configFile = null;
        try{
            configFile = new Scanner( new File( fileName ) );
        } // end try
        catch ( FileNotFoundException fileNotFoundException ){
            System.err.println( "Error opening file: "+fileName );
            System.exit( 1 );
        } // end catch       
        
        // -- Se explora el contenido del archivo de configuracion linea por linea
        while(configFile.hasNext()){
            line = configFile.nextLine();
            // -- Se obtiene el total de clusters
            if(line.contains("#Clustering")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("endCluster:")){
                        endCluster = Integer.parseInt(words[i+1]);
                    }
                }
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("initCluster:")){
                        initCluster = Integer.parseInt(words[i+1]);
                    }
                }
            }
            // -- Cantidad de Threads, depende de las capacidades de la PC.
            // -- Generalmente, para frames de 320x240 se usan 3 o 2, para frames de 240x160 se usan 4
            if(line.contains("#ThreadCount")){
                String[] words = line.split(" ");    
                for (int i = 0; i < words.length; i++) {
                    if(words[i].equals("clusters:")){ // - Para convolucion
                        threadCount = Integer.parseInt(words[i+1]);
                    }
                }
            }
        }
        configFile.close();
        
        
        Scanner input = null;
//        String fileName;        
        int i = 0;
        List<String> sections = new ArrayList<>();
        
        for(int j = initCluster; j <= endCluster; j++){
            fileName = "../clustering/Cluster"+j+"/clusterSections.txt";
            try{
                input = new Scanner( new File( fileName ) );
            } // end try
            catch ( FileNotFoundException fileNotFoundException ){
                System.err.println( "Error opening file: "+fileName );
                System.exit( 1 );
            } // end catch       
            
            frameIdx = 0;  
            i = 0;
            while(true){
                sections.clear();
                frameIdx += i;
                i = 0;
                while(input.hasNext() &&  i < threadCount){
                    sections.add(input.nextLine());
                    i++;
                }
                ClusterFrameTreads(j,sections);
                
                if(!input.hasNext())
                    break;
            }
        }
    }
    
    public void ClusterFrameTreads(int cluster,List<String> sections){
//        System.out.println("- Block --------------");

    
        System.out.println( "Starting Executor" );
        String cmd = "";
        // create ExecutorService to manage threads
        ExecutorService threadExecutor = Executors.newCachedThreadPool();
        int i = 0;
        for(String section:sections){
//            System.out.println(section);
            cmd = "ClusterFrame.exe "+cluster+" "+section +" "+(frameIdx+i);
            CommandRunner clusterFrame = new CommandRunner(cmd);
            threadExecutor.execute( clusterFrame ); // start task1        
            i++;
        }        
        
//        for(int i = initCluster; i <= endCluster; i++){
//            cmd = "ClusterContent.exe "+i+" 3 3 3 " ;
//            CommandRunner anim = new CommandRunner(cmd);
//            threadExecutor.execute( anim ); // start task1        
//        }
        
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
