package presenteurs
import java.util.Random
import scala.util.Random
import collection.mutable.ListBuffer

class MainPresenteur {

  
  
  private var mDirWalker = new Utils.DirWalker
  
  /**
   * 
   */
  private var mListProcess:ListBuffer[domaine.ProcessStatus]=
    new ListBuffer[domaine.ProcessStatus]
  
  /**
   * Permet de lancer une commande
   */
  def startNewTask(cmd:String)={
    System.exit(0)
    0
  }
  
  /**
   * 
   */
  var compteur =0
  
  
  /**
   * Quitte l'application
   */
  def exitApp()={
	System.exit(0);
  }
  
  /**
   * Affiche des informations sur l'application
   */
  def aPropos()={
    
  }
  
  /**
   * Lance une requete de recuperation de la liste des processus
   * dans le système, en cours d'exécution
   */
  def getDataSource()={
	mListProcess.clear()
	//Thread.sleep(700)
	/**
	 * Ici, ouvrir le repertoir /proc/
	 * pour chaque élément pid de ce repertoire, réaliser
	 * la lecture du fichier /proc/pid/status
	 * créer une instance de ProcessStatus
	 * l'ajouter à la liste.
	 * Ainsi, à la fin de l'exécution de la fonction, 
	 * on est sûr que le snapshot demandé est calculé
	 * Il suffit que l'uiUpdater réactualise la vue à l'aide 
	 * de de la source de données mListProcess
	 */
	
	mDirWalker.starkWalk(mListProcess)
	
  }
  
  
  /**
   * 
   */
  def pickDataSource()={
    mListProcess
  }
  
  /**
   * Demande à la vue de se raffraichir
   */
  def actualiser()={
    
	  	//Ici parcourir la table ,et inserer les elements de mListProcess
	  	  var i =0
		  for (i <- 0 until vues.LinuxProcessExplorerMainView.valuesTable.rowCount){
		   for(j <- 0 until domaine.ProcessStatus.ColumnCount)
		   {
		     compteur = compteur+1
		     vues.LinuxProcessExplorerMainView.valuesTable(i,j)=compteur+" Kb"
		   }
	  }
  }
}