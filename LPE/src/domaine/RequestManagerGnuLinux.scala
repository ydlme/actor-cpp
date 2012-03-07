package domaine

class RequestManagerGnuLinux extends RequestManager{

  def getProcessList():List[domaine.ProcessStatus]=
  {
    List(new domaine.ProcessStatus(1),new domaine.ProcessStatus(2))
  }
  
  /**
   * Renvoie le status du processus de pid passé en argument
   */
  def getProcessById(pid:Int):domaine.ProcessStatus=
  {
    new domaine.ProcessStatus(pid)
  }
}