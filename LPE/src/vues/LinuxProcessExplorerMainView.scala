package vues

import scala.swing._
import event._
import scala.actors._
import Actor._
import java.awt.Color
import java.awt.Font




import java.awt.Dimension
import javax.swing.ImageIcon
import swing._
import swing.event._
import TabbedPane._


  class MyPanel extends GridPanel(1,1) {

    val feature ="Feature non disponible encore"

    var pageTab1:Page =null

    val tabbedPane = new TabbedPane() {
         //val icon = createImageIcon("images/middle.gif")

          val panel1 = new makeTextPanel("Panel #1")

          pageTab1=new Page(
            this, "Processus", panel1,
            "Does nothing")
          {
            //mnemonic = Key.Key1

          }
          pages += pageTab1

         val panel2 = new makeTextPanel(feature)
         pages += new Page(
               this, "Performances", panel2,
              "Does twice as much nothing")
          {
              //mnemonic = Key.Key2
            }

          val panel3 = new makeTextPanel(feature)
          pages += new Page(this, "Mise en réseau", panel3,"Still does nothing")
           {
               //mnemonic = Key.Key3
            }

           val panel4 = new makeTextPanel(feature)
           {
               preferredSize = new Dimension(410, 50)
           }
           pages += new Page(this, "Utilisateurs", panel4,"Does nothing at all")
           {
               //mnemonic = Key.Key4
           }
         }

       //Add the tabbed pane to this panel.
       contents += tabbedPane

       //The following line enables to use scrolling tabs.
       tabbedPane.tabLayoutPolicy = Layout.Scroll

       // ----------------------------------------
       class makeTextPanel(text: String) extends GridPanel(1, 1) {
          val filler = new Label(text) {
               horizontalAlignment = Alignment.Center
             }
           contents += filler
         }

       /** Returns an ImageIcon, or null if the path was invalid. */
       def createImageIcon(path: String): ImageIcon = {
           //val imgURL = TabbedPaneDemo.getClass.getResource(path)
           //if (imgURL != null) {
               //return new ImageIcon(imgURL)
             //} else {
               //System.err.println("Couldn't find file: " + path)
               return null
            //}
         }
     }


/*------------------------------------------------------------------------*/
/**
 * Classe qui sert de GUI à notre process Explorer
 */
object LinuxProcessExplorerMainView extends SimpleGUIApplication {

  
  
  def top = MainFrame
  
  val mPresenteur :presenteurs.MainPresenteur=new presenteurs.MainPresenteur
  var valuesTable :Table=null
  
  /**
   * 
   */
  def getInitalValues: Array[Array[Any]] ={
    val emptyMatrix = new Array[Array[Any]](50,domaine.ProcessStatus.ColumnCount)
    emptyMatrix
  }
  
  /**
   * La frame principale de la GUI
   */
  val MainFrame= new MainFrame
  {
  
    title = Utils.Constantes.TITLE
    val dateLabel = new Label { 
      font= new Font("Serif", Font.BOLD, 14)
      }
    
    /**
     * Permet de fixer l'unité de mesure de la consommation mémoire
     */
    def getUnit()={
      " (KB)"
    }
    
    
    valuesTable = new Table(
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
    				domaine.ProcessStatus.Cpu_Time+" ms",
    				domaine.ProcessStatus.Cpu_Taux+ " %",
    				domaine.ProcessStatus.Threads))
	    		{
	      
	    		  showGrid  = true
	    		  gridColor = Color.BLUE
	    		  foreground= Color.BLACK
	    		  font = new Font("MS SANS Serif", Font.CENTER_BASELINE, 12)
	    		  
	    		}


    /**
     * Setting the header font style
     */
    valuesTable.peer.getTableHeader().setFont(new Font("Serif", Font.BOLD, 12))
    
    /**
     * Adding buttons to interaction
     */
    val updateButton 	= new Button 	{text = Utils.Constantes.LABEL_UPDATE;name="btnupdate"}
    val killButton      = new Button    {text = Utils.Constantes.LABEL_KILL;name="btnkill"  }
    //val processExpLabel = new Label 	{text = Utils.Constantes.LABEL_PROCESS}
    
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
    
    contents = new BoxPanel(Orientation.Vertical)  
    {
        contents += dateLabel
        contents += valuesTable
        val tab = new MyPanel
        contents += tab
        
        var sp = new BoxPanel(Orientation.Vertical)  
        {
          contents+= new ScrollPane(valuesTable)
          contents+= new FlowPanel
          {
            contents+= updateButton
            //contents+= processExpLabel
            contents+= killButton
          }
        }

        tab.pageTab1.content = sp
      }

      /**
       * Abonnement aux events
       */
      listenTo(updateButton)
      listenTo(killButton)

      /**
       * Les reaction des buttons
       */
      reactions +=
        {
          case ButtonClicked(button) =>
            /*Ici mettre l'action qui sera déclanchée lors du clique*/
            Console.println("Btn clicked : "+button.name)
            button.enabled = false
            uiUpdater ! "ok"

        }
			
    
    
    /**
     * @END
     * */
    
	
	/**
	 * Acteur qui permet de mettre à jour la vue
	 */
	val uiUpdater = new Actor 
	{
	  def act = {
	    loop
	    {
	      react{
	        case "ok" => 
	          					
	          					val format = "dd/MM/yy H:mm:ss"; 
	          					val formater = new java.text.SimpleDateFormat( format ); 
	          					val date = new java.util.Date(); 
 
	        					dateLabel.text= Utils.Constantes.LABEL_DATE+formater.format( date )
	          					updateTable("ok")
	          					updateButton.enabled = true
	          					killButton.enabled 	 = true
	      }
	    }
	  }
	  
	  override def scheduler() = new SingleThreadedScheduler
	}
	
	uiUpdater.start()
	
	
	/**
	 * 
	 */
	val looper = new Actor 
	{
	  def act = {
	    
	    while(true){
	      
	      //@TODO, faire un appel au présenteur pour calculer les données
	      mPresenteur.getDataSource()
	      uiUpdater ! "ok"
	    }
	  }
	  
	  //override def scheduler() = new SingleThreadedScheduler
	}
	looper.start()
	

	/**
	 * 
	 */
	def updateTable(value :String)
	{
	      //Console.println("Hello from update panel with row count = "+ valuesTable.rowCount)
	      mPresenteur.actualiser()
	}
  }
  
}