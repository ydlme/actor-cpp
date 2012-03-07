package domaine

trait RequestManager {

  /**
   * Permet de recupere la liste des processus en cours d'exécution 
   * dans les système
   */
  def getProcessList():List[domaine.ProcessStatus]
  
  /**
   * Renvoie le status du processus de pid passé en argument
   */
  def getProcessById(pid:Int):domaine.ProcessStatus
  
}