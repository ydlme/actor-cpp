package vues

import scala.swing._
import event._
import scala.actors._
import Actor._
import java.awt.Color
/**
 * Classe qui sert de GUI à notre process Explorer
 */
object LinuxProcessExplorerMainView extends SimpleGUIApplication {

  def top = MainFrame
  
  /**
   * 
   */
  def getInitalValues: Array[Array[Any]] ={
    val emptyMatrix = new Array[Array[Any]](10,domaine.ProcessStatus.ColumnCount)
    emptyMatrix
  }
  
  /**
   * 
   */
  val MainFrame= new MainFrame
  {
  
    title = "Linux Process Explorer, thanks Scala"
    val dateLabel = new Label { text= "Last updated: ***"}
    
    val valuesTable = new Table(
    		getInitalValues,
    		
    		Array(	
    				domaine.ProcessStatus.User,
    				domaine.ProcessStatus.PID,
    				domaine.ProcessStatus.Vm_Data,
    				domaine.ProcessStatus.Vm_Swap,
    				domaine.ProcessStatus.Vm_Rss,
    				domaine.ProcessStatus.Vm_Exe,
    				domaine.ProcessStatus.Vm_Stk,
    				domaine.ProcessStatus.Vm_Peak))
	    		{
	      
	    		  showGrid  = true
	    		  gridColor = Color.BLUE
	    		}

    val updateButton 	= new Button 	{text = "Update"}
    val killButton      = new Button    {text = "Kill"  }
    val processExpLabel = new Label 	{text = "Process in system"}
    
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
	  
	  Console.println("Hello from update panlel with row count = "+ valuesTable.rowCount)
	 
	  val i =0
	  for (i <- 0 until valuesTable.rowCount){
	   for(j <- 0 until domaine.ProcessStatus.ColumnCount)
	   {
	     valuesTable(i,j)=j
	   }
	  }
	}
  }
  
}