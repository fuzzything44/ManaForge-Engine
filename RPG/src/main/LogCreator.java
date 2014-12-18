package main;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.Calendar;

public class LogCreator {
// A class for creating crash logs and such.
// crashLog takes the exception and Play instance. If no play instance, it can take just the exception
	private LogCreator() {}
	public static void crashLog(Exception e) {
		crashLog(e, null);
	}
	
	public static void crashLog(Exception e, Play p) {
		try {
			String timeStamp = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(Calendar.getInstance().getTime());			
			// Gets the current time as a String. Example:
			// 2014-12-18_15-58-26
			// Comes from running it at December 18, 2014 at 3:58 PM. 26 seconds after that time.
			String logName = "log-" + timeStamp + ".txt";
			
			BufferedWriter logFile = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(logName) ) );
			// Writer to create the log file
			
			// Writing log file.
			logFile.write("Crash Log:");
			logFile.newLine();
			
			logFile.write(e.toString());
			logFile.newLine();
			// What type of exception it is
			
			StackTraceElement[] stackTrace = e.getStackTrace();
			for (int i = 0; i < stackTrace.length; i++) {
				logFile.write("\t" + stackTrace[i]);
				logFile.newLine();
			}
			// Writing the entire stack trace.
			
			if (p != null) {
				logFile.write("Chunks loaded: " + p.relevantChunks.size() );
				logFile.newLine();
				// Writing how many chunks are loaded.
				
				int actorsLoaded = 0;
				for (int i = 0; i < p.relevantActors.size(); i++) {
					actorsLoaded += p.relevantActors.get(i).size();
				}
				logFile.write("Actors loaded: " + actorsLoaded);
				// Calculating and writing how many actors are loaded
				
			} else {
				// If no Play instance was given, say so.
				logFile.write("Nonstandard error - no Play loaded");
			}
			
			logFile.newLine();
			
			// Anything else we want to have in log files?
			logFile.close();
		} catch (Exception fail) {
			fail.printStackTrace();
		}

	}	// End crashLog
		
}
