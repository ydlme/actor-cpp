package domaine



import collection.mutable.HashMap
import scala.io.Source


object StatusReader {


  val prefix = "proc.txt"

  /**
   * table des symboles de l'exécutable instrumenté
   */
  private val table_symbs : HashMap [String,String]= new HashMap[String,String]

  def ReadStatus(pid:Int)={

   table_symbs.clear()

    //val path = "/proc/"+pid+"status"
    val file = Source.fromFile(prefix)
    val lines = file.getLines

    lines.foreach(line=>
    {
      val tokens =line.replaceAll("kB","").split(":")

      table_symbs+=(tokens(0)->tokens(1))
    })
    Console.println("Lines: "+lines.length)


    table_symbs
  }
}