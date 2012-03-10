package domaine



import collection.mutable.HashMap
import scala.io.Source


object StatusReader {


  val prefix = "proc.txt"

  private val table_symbs : HashMap [String,String]= new HashMap[String,String]

  def ReadStatus(path:String)={

   table_symbs.clear()

   var result = new ProcessStatus(0)
    //val path = "/proc/"+pid+"status"
    val file = Source.fromFile(path)
    val lines = file.getLines

    lines.foreach(line=>
    {
      val tokens =line.replaceAll("kB","").split(":")

      try{
    	  table_symbs+=(tokens(0)->tokens(1))
      }
      catch{
        case e:Exception => //println("Invalid process file")
      }
      
    })
    Console.println("Lines: "+lines.length)


    result.VmData=0
    result.VmExe=0
    result.VmHWM=0
    result.VmLib=0
    result.VmLock=0
    result.VmPeak=0
    result.VmPTE=16
    result.VmRSS=0
    result.VmStk=0
    result.VmSwap=0
    
    result
  }
}