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
  var strUnite:String 	="KB"
  var intUnite:Int      =1
  var intRafPeriod      =1000
  
  /*@start MENU 1*/
  var cmiZombieProcessItem :CheckMenuItem=null
  var cmiSystemProcessItem :CheckMenuItem=null
  var cmiGroupeProcessItem :CheckMenuItem=null
  var cmiStatsProcessItem 	:CheckMenuItem=null
  
  /*@end*/
  
  /*@start MENU 2*/
  var rdiFrequenceHaute	  :RadioMenuItem=null
  var rdiFrequenceMoyenne :RadioMenuItem=null
  var rdiFrequenceBasse   :RadioMenuItem=null
  var rdiUniteKb 	  	  :RadioMenuItem=null
  var rdiUniteGb          :RadioMenuItem=null
  var rdiUniteMb          :RadioMenuItem=null
  var rdis=List(
		  		rdiFrequenceBasse,
		  		rdiFrequenceHaute,
		  		rdiFrequenceMoyenne,
		  		rdiUniteGb,
		  		rdiUniteKb,
		  		rdiUniteMb
		  		)
  /*@end*/
  
  
  
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
    				domaine.ProcessStatus.User,
    				domaine.ProcessStatus.PID,
    				domaine.ProcessStatus.Image,
    				domaine.ProcessStatus.Vm_Data,
    				domaine.ProcessStatus.Vm_Swap,
    				domaine.ProcessStatus.Vm_Rss,
    				domaine.ProcessStatus.Vm_Exe,
    				domaine.ProcessStatus.Vm_Stk,
    				domaine.ProcessStatus.Vm_Peak,
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
    val updateButton 	= new Button 	
    {text = Utils.Constantes.LABEL_UPDATE;name=Utils.Constantes.BTN_UPDATE}
    val killButton      = new Button    
    {text = Utils.Constantes.LABEL_KILL;name=Utils.Constantes.BTN_KILL}
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
	
	/*@start Menu options*/
	cmiZombieProcessItem = new CheckMenuItem("Afficher les processus zombie")
	cmiSystemProcessItem = new CheckMenuItem("Afficher les processus système")            
	cmiGroupeProcessItem = new CheckMenuItem("Afficher les processus par groupe")      
	cmiStatsProcessItem  = new CheckMenuItem("Afficher les statistiques") 
	val mutexTypeProcess = new ButtonGroup(cmiGroupeProcessItem,cmiSystemProcessItem,cmiZombieProcessItem)
    contents += new Menu("Options"){
	  
	  contents ++= mutexTypeProcess.buttons
	
	  contents += new Separator        
	  contents += cmiStatsProcessItem
	  }      
	/*@end*/
	
    /* start Menu affichage */
    rdiFrequenceHaute 	= new RadioMenuItem("Haute"){name="rbfh"}
    rdiFrequenceMoyenne = new RadioMenuItem("Moyenne"){name="rbfm"}
    rdiFrequenceBasse 	= new RadioMenuItem("Basse"){name="rbfb"}
    val mutexFrequence  = new ButtonGroup(rdiFrequenceBasse,rdiFrequenceHaute,rdiFrequenceMoyenne)		  
    
    rdiUniteKb = new RadioMenuItem("KB"){name ="rbuk"}
    rdiUniteMb = new RadioMenuItem("MB"){name ="rbum"}
    rdiUniteGb = new RadioMenuItem("GB"){name ="rbug"}
    val mutexUnite = new ButtonGroup(rdiUniteKb,rdiUniteMb,rdiUniteGb)		  
    
	contents += new Menu("Affichage"){
    		  
    		  contents += new MenuItem("Fréquence rafraichissement")
			  contents += new Separator        
			  
    		  contents ++=mutexFrequence.buttons
    		  contents += new Separator        
			  
			  contents += new MenuItem("Unité")
    		  contents += new Separator        
			  contents ++=mutexUnite.buttons
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

      /*@start abonement aux buttons*/
      listenTo(updateButton)
      listenTo(killButton)
      listenTo(rdiFrequenceHaute)
      listenTo(rdiFrequenceBasse)
      listenTo(rdiFrequenceMoyenne)
      listenTo(rdiUniteGb)
      listenTo(rdiUniteKb)
      listenTo(rdiUniteMb)
      /*@end*/
      
      /**
       * Les reaction des buttons
       */
      reactions +=
        {
          case ButtonClicked(button) =>
            /*Ici mettre l'action qui sera déclanchée lors du clique*/
            
            button.name match
            {
              case Utils.Constantes.BTN_UPDATE =>
              case Utils.Constantes.BTN_KILL   => mPresenteur.killProcess(1111)
              case "rbfh" => intRafPeriod=1000
              case "rbfm" => intRafPeriod=10000
              case "rbfb" => intRafPeriod=60000
              case "rbum" => strUnite="Mb";intUnite=1024
              case "rbuk" => strUnite="Kb";intUnite=1
              case "rbug" => strUnite="Gb";intUnite=1024*1024
              
            }
            Console.println("Btn clicked : "+button.name)
            //button.enabled = false
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
	      
	      Thread.sleep(intRafPeriod)
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