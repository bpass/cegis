		menu session "[S]ession" {
			sprefer "[P]references ...|Ctrl<Key>P|Ctl+P" {
				preferenceeditor;
			}
			sconfig "[C]onfiguration ...|Ctrl<Key>C|Ctl+C" { 
				configurationeditor;
			}
			sstatus	"[S]ession Log ...|Ctrl<Key>S|Ctl+S" { 
				display statuswindow;
			}
                        sprocess "Active Process List..." {
                                display processwindow;
                        }
			scommands "Commands ..."  {
				display commandwindow;
			}
			slogcomment "Enter [L]og message...|Ctrl<Key>L|Ctl+L" {
				display logenter;
			}

			separator;
			
			bstart "Start Recording Batch Commands..." { startbatch; }
			bopen "Open Batch Command File ..." {
				if ( $bcffile.QueryForFilename() ) { BatchCommandFileOpen $bcffile;}
				}
			bview  "View Offline Batch Queue ..." {viewbatchlist; }

			separator;
			
			sessionflip  "Flip Icons" {
                                if ( $iconlayout == "vertical" ) {
                                        set iconlayout = "horizontal";
                                }
                                else {
                                        set iconlayout = "vertical";
                                }
				recompute_sizes($iconlayout);
		                if ($iconpaneldisplay == "Standard Icons") 
					$iconpanel.geometryset(0, 0, $iconw, $iconh);
		                else
                		if ($iconpaneldisplay == "Standard Icons (no labels)") 

					$iconpanelnotext.geometryset(0, 0, $iconw, $iconh);
		                else
                		if ($iconpaneldisplay  == "Small Icons") 

					$iconpanelsmall.geometryset(0, 0, $iconw, $iconh);
				else
					$iconpanelnoicons.geometryset(0, 0, $iconw, $iconh);
				viewer tile at $tile_x, $tile_y
                                        size getpref("viewer" "window_xsize"),
                                        getpref("viewer" "window_ysize");
			}
			
			scleanup "Tile Viewers"  {
				$iconpanel.geometryset(0, 0, $iconw, $iconh);
				viewer tile at $tile_x, $tile_y
					size $tile_width, $tile_height;
                        }		
			separator;
			
			sviewkillall "Close All Viewers" {viewer destroyall;}
			separator;
			
			menu sessionmainmenu "Main" {
				sessionmainlogo "IMAGINE Credits..."	
					{ display creditframe;}
				sessionmainviewer "Start IMAGINE Viewer..."	
					{ viewer create at $tile_x, $tile_y
					size getpref("viewer" "window_xsize"),
					getpref("viewer" "window_ysize");}
				sessionmainimport "Import/Export..."	
					{ load "import.eml";}
				sessionmaindataprep "Data Preparation..."	
					{ load "dataprep.eml";}
				sessionmainmapcomp "Map Composer..."	
					{ load "mapcomp.eml";}
				<imadvan> sessionmainimageinterp 
					"Image Interpreter..."	
					{ load "ii_toplevel.eml";}
				sessionmainimagecatalog "Image Catalog..."	
					{ catalog; } 
				<imadvan> sessionmainimageclass 
					"Image Classification..."	
					{ load "classification.eml"; } 
				<imadvan> sessionmainspatialmodeler 
					"Spatial Modeler..."	
					{ load "sm_toplevel.eml"; } 
				sessionmainvector "Vector..."	
					{ load "vectorutil.eml"; } 
				<imradar> sessionmainradar "Radar..."	
					{ load "radar_top.eml"; } 
				<imvirtual> sessionmainvgis "Virtual GIS..."	
					{ load "virtualgis_top.eml"; } 
				<imorthobase> sessionmainorthobase "OrthoBASE..."	
					{ blocktool;}
				<amortmx> sessionmainorthomax "OrthoMAX..."	
					{ amorthomax;} 
				<gsatcor> sessionmainatcor "Atcor..."	
					{ atcor;} 
				<gsebis> sessionmainebis "EBIS..."	
					{ ebis;} 
				<aaiaasap> sessionmainsubpix 
					"Sub-Pixel Classifier..."	
					{ load "aaiaasap.eml";} 
				<imhifi> sessionmainhifi "Stereo Analyst..."	
					{ hifi toplevel;}
			}


			menu sessiontoolsmenu "Tools" {
				sessiontoolstextedit "Edit Text Files..."	
					{ editor;}
				sessiontoolsdsceditor "Edit Raster Attributes..."
					{ dsceditor; }
				sessiontoolsdataview "View Binary Data..."	
					{ dataview;}
				sessiontoolshfaview "View IMAGINE HFA File Structure..."	
					{ hfaview;}
				separator;
				sessiontoolsannoinfo "Annotation Information..."
					{ annotationinfo;}
				sessiontoolsfilemanager "Image Information..."
					{ imageinfo;}
				sessiontoolsvectorinfo "Vector Information..."
					{ covinfo;}
				sessiontoolsimagecommand "Image Command Tool..."
					{ imagecommand_ui;}
				separator;
				sessionutilcoordcalc "Coordinate Calculator..."
					{ coordcalc; }
				sessiontoolsmovie "Create/Display Movie Sequences..."
					{ movie; }

				sessiontoolsvueplayer "Create/Display Viewer Sequences..."
					{ vueplayer; }

				sesimagedrape "Image Drape..."
					info "Start Image Drape with Displayed DEM"
					{ 
							job imagedrape create staticview
								at 0 0 size 500 500; 
					}

				<unix> separator;
				<unix> sessiontoolsfontfm "Manage PostScript and TrueType Fonts..."
					{ spawn fontfm -cf getenv ( "IMAGINE_HOME" ) +"/etc/fonts/fontfm.cfg"; }
				<imtool> toolsemlview "View EML ScriptFiles..."
					info "View and Edit EML ScriptFiles..."
					{emlview;}
			}
			menu sessionutilmenu "Utilities" {
				sessionutiljpegcomp "JPEG Compress Images..."
					{ load "jpegcompress.eml"; }
				sessionutiljpegdecomp "Decompress JPEG Images..."
					{ load "jpegdecompress.eml"; }
				@"compression_session.inc"
				separator;
				sessionutilpixeltotable "Convert Pixels to ASCII..."
					{ spawn pixeltotable; }
				sessionutiltabletopixel "Convert ASCII to Pixels..."
					{ spawn tabletopixel; }
				separator;
				sessionutilrastoant "Convert Images to Annotation..."
					{ rastertoantui; }
				sessionutilrastoant "Convert Annotation to Raster..."
					{ anttorasterui; }
				separator;
				sessionutilchips "Create/Update Image Chips..."
					{ load "chipmaker.eml"; }
				<unix> separator;
				<unix> sessionutilcdmount "Mount/Unmount CDROM..."
					{ cdmountutil(); }
				<unix> sessionutilparallel "Create Lowercase Parallel Links..."
					{ load "parallel.eml"; }
				separator;
				sessionutilfonttablemaker "Create Font Tables..."
					{ fonttablemaker; }
				separator;
				sessionutilimgcomp "Compare Images..."
					{ load "imgcompare.eml"; }
				separator;
				sessionutilconfrf "Reconfigure Raster Formats"
					{ syncjob configure_rf;
					  message ( "Reconfigure of Raster Formats DLLs is now complete.  The changes will take effect the next time IMAGINE is restarted." ); }
				sessionutilconfvf "Reconfigure Vector Formats"
					{ syncjob configure_vf; 
					  message ( "Reconfigure of Vector Formats DLLs is now complete.  The changes will take effect the next time IMAGINE is restarted." ); }
				<imnitf,imtaco2> separator;
                                <imtaco2> sessionutiltaco2 "Manage TACO2 Communications..."
                                        { load "taco2.eml"; }
                                <imnitf> sessionutilnitfedit "NITF Editor..."
                                        { nitfedit "nitf.eml"; }
			}                  					
			menu shelpmenu "Help" {
				shelpicon "Help for Icon Panel..."
					{ showhelp "imagine", "iconpanel", "";}
				shelpond "IMAGINE Online Documentation..."
					{ showhelp "titles" "Main_ttl" "tag";}
				shelpver "IMAGINE Version..."
					{ showversion; }
				shelpdll "IMAGINE DLL Information..."	
					{ versiontool;}
			}			

			separator;
			
			<pc> sprop "Properties..." {
				job imagineprop;
			}
			<pc> separator;

            squit "Exit IMAGINE...|Ctrl<Key>Q|Ctl+Q"
					{display checkquitframe;}
		
		}
		
		

		menu mainmenu "Main" {
			mainlogo "IMAGINE Credits..."	
				{ display creditframe;}
			mainviewer "Start IMAGINE Viewer..."	
				{ viewer create at $tile_x, $tile_y
				size getpref("viewer" "window_xsize"),
				getpref("viewer" "window_ysize");}
			mainimport "Import/Export..."	
				{ load "import.eml";}
			maindataprep "Data Preparation..."	
				{ load "dataprep.eml";}
			mainmapcomp "Map Composer..."	
				{ load "mapcomp.eml";}
			<imadvan> mainimageinterp 
				"Image Interpreter..."	
				{ load "ii_toplevel.eml";}
			mainimagecatalog "Image Catalog..."	
				{ catalog; } 
			<imadvan> mainimageclass 
				"Image Classification..."	
				{ load "classification.eml"; } 
			<imadvan> mainspatialmodeler 
				"Spatial Modeler..."	
				{ load "sm_toplevel.eml"; } 
			mainvector "Vector..."	
				{ load "vectorutil.eml"; } 
			<imradar> mainradar "Radar..."	
				{ load "radar_top.eml"; } 
			<imvirtual> mainvgis "Virtual GIS..."	
				{ load "virtualgis_top.eml"; } 
			<imorthobase> mainorthobase "OrthoBASE..."	
				{ blocktool;}
			<amortmx> mainorthomax "Orthomax..."	
				{ amorthomax;} 
			<gsatcor> mainatcor "Atcor..."	
				{ atcor;} 
			<gsebis> mainebis "EBIS..."	
				{ ebis;} 
			<aaiaasap> mainsubpix 
				"Sub-Pixel Classifier..."	
				{ load "aaiaasap.eml";} 
			<imhifi> mainhifi "Stereo Analyst..."	
				{ hifi toplevel;}
		}

		menu toolsmenu "[T]ools" {
			toolstextedit "Edit Text Files..."	
				{ editor;}
			toolsdsceditor "Edit Raster Attributes..."
				{ dsceditor; }
			toolsdataview "View Binary Data..."	
				{ dataview;}
			toolshfaview "View IMAGINE HFA File Structure..."	
				{ hfaview;}
			separator;
			toolsvectorinfo "Annotation Information..."
				{ annotationinfo;}
			toolsfilemanager "Image Information..."
				{ imageinfo;}
			toolsvectorinfo "Vector Information..."
				{ covinfo;}
			toolsimagecommand "Image Command Tool..."
				{ imagecommand_ui;}
			separator;
			utilcoordcalc "Coordinate Calculator..."
				{ coordcalc; }
			toolsmovie "Create/Display Movie Sequences..."
				{ movie; }
			toolsvueplayer "Create/Display Viewer Sequences..."
				{ vueplayer; }

                	toolsimagedrape "Image Drape..."
                                info "Start Image Drape with Displayed DEM"
                    { 
						job imagedrape create staticview
							at 0 0 size 500 500;
				}

			<unix> separator;
			<unix> toolsfontfm "Manage PostScript and TrueType Fonts..."
				{ spawn fontfm -cf getenv ( "IMAGINE_HOME" ) +"/etc/fonts/fontfm.cfg"; }
			<imtool> toolsemlview "View EML ScriptFiles..."
				info "View and Edit EML ScriptFiles..."
				{emlview;}
		}
		menu utilmenu "[U]tilities" {
			utiljpegcomp "JPEG Compress Images..."
				{ load "jpegcompress.eml"; }
			utiljpegdecomp "Decompress JPEG Images..."
				{ load "jpegdecompress.eml"; }
			@"compression_utils.inc"
			separator;
			utilpixeltotable "Convert Pixels to ASCII..."
				{ spawn pixeltotable; }
			utiltabletopixel "Convert ASCII to Pixels..."
				{ spawn tabletopixel; }
			separator;
			utilrastoant "Convert Images to Annotation..."
				{ rastertoantui; }
			utilrastoant "Convert Annotation to Raster..."
				{ anttorasterui; }
			separator;
			utilchips "Create/Update Image Chips..."
				{ load "chipmaker.eml"; }
			<unix> separator;
			<unix> utilcdmount "Mount/Unmount CDROM..."
				{ cdmountutil(); }
			<unix> utilparallel "Create Lowercase Parallel Links..."
				{ load "parallel.eml"; }
			separator;
			utilfonttablemaker "Create Font Tables..."
				{ fonttablemaker; }
			separator;
			sessionutilimgcomp "Compare Images..."
				{ load "imgcompare.eml"; }
			separator;
			utilconfrf "Reconfigure Raster Formats"
				{ syncjob configure_rf;
				  message ( "Reconfigure of Raster Formats DLLs is now complete.  The changes will take effect the next time IMAGINE is restarted." ); }
			utilconfvf "Reconfigure Vector Formats"
				{ syncjob configure_vf; 
				  message ( "Reconfigure of Vector Formats DLLs is now complete.  The changes will take effect the next time IMAGINE is restarted." ); }
			<imnitf,imtaco2> separator;
			<imtaco2> utiltaco2 "Manage TACO2 Communications..."
				{ load "taco2.eml"; }
			<imnitf> utilnitfedit "NITF Editor..."
				{ nitfedit "nitf.eml"; }
		}		
        menu mymenu "[E]xtra" 
        {
        	
	        
	        trans "Header Transfer Tool"
	        {
				spawn "transferHeader.exe";
			}
        }	
		menu helpmenu "[H]elp" {

			shelpicon1 "Help for Icon Panel..."
				{ showhelp "imagine", "iconpanel", "";}
			shelpond1 "IMAGINE Online Documentation..."
				{ showhelp "titles" "Main_ttl" "tag";}
			shelpver1 "IMAGINE Version..."
				{ showversion; }
			shelpdll1 "IMAGINE DLL Information..."	
				{ versiontool;}
		}

