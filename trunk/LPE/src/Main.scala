

object Main {
  def main(args : Array[String]) : Unit = {
    
    Console.println("Hello to scala 2.9.1");
    Console.println(new domaine.ProcessStatus(1122))
    
    vues.LinuxProcessExplorerMainView.main(args)
  }
}
