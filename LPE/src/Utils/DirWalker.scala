package Utils


import java.io.{FileSystem, File}
import collection.mutable.ListBuffer

class DirWalker {

  val RootPath ="I:\\Android\\src\\android\\wch"
  val ProcPath="/proc/%s/status"
  val Height   =2

  val keyWord ="class"
  var count=0

  def getKeyWordCount()={
    count
  }
  
  def starkWalk(mListProcess:ListBuffer[domaine.ProcessStatus])={
    Console.println("Walking directory : "+RootPath)
    val fileRoots = new File(RootPath).listFiles()

    fileRoots.foreach(
        file =>
	    {   
	      if (file.isFile && file.canRead && file.getAbsolutePath.contains(".java") )
	      {
	        Console.println(file)
	        mListProcess+=domaine.StatusReader.ReadStatus(file.getAbsolutePath())
	      }
	    }
	  )
  }
}