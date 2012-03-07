package vues

import scala.swing._
import event._
import scala.actors._
import Actor._
import java.awt.Color
import java.awt.Font
/**
 * Classe qui sert de GUI à notre process Explorer
 */
object LinuxProcessExplorerMainView extends SimpleGUIApplication {

  def top = MainFrame
  
  val mPresenteur :presenteurs.MainPresenteur=new presenteurs.MainPresenteur
  
  
  /**
   * 
   */
  def getInitalValues: Array[Array[Any]] ={
    val emptyMatrix = new Array[Array[Any]](10,domaine.ProcessStatus.ColumnCount)
    emptyMatrix
  }
  
  /**
   * La frame principale de la GUI
   */
  val MainFrame= new MainFrame
  {
  
    title = Utils.Constantes.TITLE
    val dateLabel = new Label { text= Utils.Constantes.LABEL_DATE+"12/12/2012"}
    
    /**
     * Permet de fixer l'unité de mesure de la consommation mémoire
     */
    def getUnit()={
      " (KB)"
    }
    
    
    val valuesTable = new Table(
    		getInitalValues,
    		
    		Array(	
    				domaine.ProcessStatus.User+getUnit(),
    				domaine.ProcessStatus.PID+getUnit(),
    				domaine.ProcessStatus.Image,
    				domaine.ProcessStatus.Vm_Data+getUnit(),
    				domaine.ProcessStatus.Vm_Swap+getUnit(),
    				domaine.ProcessStatus.Vm_Rss+getUnit(),
    				domaine.ProcessStatus.Vm_Exe+getUnit(),
    				domaine.ProcessStatus.Vm_Stk+getUnit(),
    				domaine.ProcessStatus.Vm_Peak+getUnit(),
    				domaine.ProcessStatus.Cpu_Time,
    				domaine.ProcessStatus.Cpu_Taux,
    				domaine.ProcessStatus.Threads))
	    		{
	      
	    		  showGrid  = true
	    		  gridColor = Color.BLUE
	    		  foreground= Color.BLACK
	    		  font = new Font("Serif", Font.BOLD, 14)
	    		  
	    		}


    /**
     * Setting the header font style
     */
    valuesTable.peer.getTableHeader().setFont(new Font("Serif", Font.BOLD, 12))
    
    /**
     * Adding buttons to interaction
     */
    val updateButton 	= new Button 	{text = Utils.Constantes.LABEL_UPDATE}
    val killButton      = new Button    {text = Utils.Constantes.LABEL_KILL  }
    val processExpLabel = new Label 	{text = Utils.Constantes.LABEL_PROCESS}
    
    /**
     *@BEGIN
     * Fixe le menu de la GUI
     */
    
    /**
     * Fichier
     */
    menuBar = new MenuBar {     
	contents += new Menu("Fichier") {         
			contents += new MenuItem(Action("Nouvelle tâche")
			{
			    mPresenteur.startNewTask("bash")
			})
			
			contents += new MenuItem(Action("Quitter") 
			    { 
					mPresenteur.exitApp()
			    }) 
			}      
			
	/**
	 * Options
	 */
	contents += new Menu("Options"){
	  contents += new CheckMenuItem("Afficher les processus zombie")
	  contents += new CheckMenuItem("Afficher les processus système")            
	  contents += new CheckMenuItem("Afficher les processus par groupe")      
	            
	  //contents += new MenuItem(Action("Action item") { println(title) }) 
	  contents += new Separator        
	  contents += new CheckMenuItem("Afficher les statistiques")       
	  }      
	
	
    /**
     * Affichage
     */
    contents += new Menu("Affichage"){
    		  
    		  contents += new MenuItem("Fréquence rafraichissement")
			  contents += new RadioMenuItem("Haute")
    		  contents += new RadioMenuItem("Moyenne")
    		  contents += new RadioMenuItem("Basse")
    		  contents += new Separator        
			  
			  contents += new MenuItem(Action("Rafraichir") { println(title) }) 
			  contents += new Separator        
			  contents += new MenuItem("Unité")
			  contents += new RadioMenuItem("KB")
    		  contents += new RadioMenuItem("MB")
    		  contents += new RadioMenuItem("GB")
    		  
			  //contents += new CheckMenuItem("Check me")       
			  }
    
    /**
     * A propos
     */
    contents += new Menu("?"){
    		  contents += new MenuItem(Action("A propos") { 
    		    println(title)
    		    mPresenteur.aPropos()
    		    }) 
    		  contents += new MenuItem(Action("Aides")    { println(title) }) 
    		  
			  }
    }		
			
    
    
    /**
     * @END
     * */
	contents 			= new BoxPanel(Orientation.Vertical)  {
						  contents += dateLabel
						  contents += valuesTable
						  contents += new ScrollPane(valuesTable)
						  
						  contents += new FlowPanel
						  {
						    contents+= updateButton
						    contents+= processExpLabel
						    contents+= killButton
						  }
	}
	
	/**
	 * Abonnement aux events
	 */
	listenTo(updateButton)
	
	/**
	 * Les reaction des buttons
	 */
	reactions +=
	{
	  case ButtonClicked(button) =>
	    /*Ici mettre l'action qui sera déclanchée lors du clique*/
	    uiUpdater ! "colonne1"
	    button.enabled = false
	}
	
	/**
	 * Acteur qui permet de mettre à jour la vue
	 */
	val uiUpdater = new Actor 
	{
	  def act = {
	    loop
	    {
	      react{
	        case(s:String) => 
	          					Console.println("Hello from colonne1")
	          					updateTable("colonne1")
	          					updateButton.enabled = true
	      }
	    }
	  }
	  
	  override def scheduler() = new SingleThreadedScheduler
	}
	
	uiUpdater.start()
	
	def updateTable(value :String)
	{
	  
	  
	  //while(true)
	  {
	      //Thread.sleep(1000);
	      Console.println("Hello from update panel with row count = "+ valuesTable.rowCount)
	 
		  var i =0
		  for (i <- 0 until valuesTable.rowCount){
		   for(j <- 0 until domaine.ProcessStatus.ColumnCount)
		   {
		     valuesTable(i,j)=j
		   }
	  }
	  }
	  
	}
  }
  
}