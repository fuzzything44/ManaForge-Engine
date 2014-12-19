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
	
	public static String logLocation = "";
	// String for where the log should be generated. Ex. C:/folder/anotherfolder/

	// crashLog makes logs on crashes. Has exception
	public static void crashLog(Exception e) {
		try {
			String timeStamp = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(Calendar.getInstance().getTime());			
			// Gets the current time as a String. Example:
			// 2014-12-18_15-58-26
			// Comes from running it at December 18, 2014 at 3:58 PM. 26 seconds after that time.
			String logName = logLocation + "crash-" + timeStamp + ".txt";
			
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
			
			// Anything else we want to have in log files?
			logFile.close();
		} catch (Exception fail) {
			fail.printStackTrace();
		}

	}	// End crashLog
	
	public static void performanceLog(Play p) {
		try {
			String timeStamp = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(Calendar.getInstance().getTime());			
			// Gets the current time as a String. Example:
			// 2014-12-18_15-58-26
			// Comes from running it at December 18, 2014 at 3:58 PM. 26 seconds after that time.
			String logName = logLocation + "performance-" + timeStamp + ".txt";

			BufferedWriter logFile = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(logName) ) );
			// Writer to create the log file
			
			logFile.write("Performance log:");
			logFile.newLine();
			logFile.write("Chunks: " + p.relevantChunks.size());
			logFile.newLine();
			// More on performance logs here. We should figure out what we all want.
			
			
			logFile.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}	// End performanceLog
}
