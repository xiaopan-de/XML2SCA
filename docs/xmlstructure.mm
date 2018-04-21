<map version="1.0.1">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1523880942884" ID="ID_1075837836" MODIFIED="1523884904070">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p style="text-align: center">
      XML root
    </p>
    <p style="text-align: center">
      &lt;SCSCA_Project&gt;
    </p>
  </body>
</html></richcontent>
<font NAME="SansSerif" SIZE="20"/>
<icon BUILTIN="gohome"/>
<node COLOR="#0033ff" CREATED="1523880963525" HGAP="19" ID="ID_188966571" MODIFIED="1523890364405" POSITION="right" VSHIFT="-40">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      Modules : &lt;module&gt;s
    </p>
  </body>
</html>
</richcontent>
<edge STYLE="sharp_bezier" WIDTH="8"/>
<linktarget COLOR="#b0b0b0" DESTINATION="ID_188966571" ENDARROW="Default" ENDINCLINATION="-36;79;" ID="Arrow_ID_1041239702" SOURCE="ID_23097189" STARTARROW="None" STARTINCLINATION="292;0;"/>
<font NAME="SansSerif" SIZE="18"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="35" VALUE_WIDTH="223"/>
<attribute NAME="name" VALUE=":module name"/>
<attribute NAME="type" VALUE=":module type [sc] or [sca]"/>
<attribute NAME="moc" VALUE=":model of computation,[ tdf], [eln],[lsf]"/>
<attribute NAME="desc" VALUE=":description"/>
<node COLOR="#00b439" CREATED="1523881460227" HGAP="85" ID="ID_1618951017" MODIFIED="1523890489588" VSHIFT="215">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      input ports : &lt;port_in&gt;s
    </p>
  </body>
</html>
</richcontent>
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="36" VALUE_WIDTH="277"/>
<attribute NAME="name" VALUE=": port name"/>
<attribute NAME="ptype" VALUE=": port typ: e.g. sca_tdf::sca_in"/>
<attribute NAME="dtype" VALUE=": data type"/>
<attribute NAME="desc" VALUE=": description"/>
</node>
<node COLOR="#00b439" CREATED="1523881461451" HGAP="85" ID="ID_893545623" MODIFIED="1523890492614" TEXT="output ports :&lt;port_out&gt;s" VSHIFT="43">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="36" VALUE_WIDTH="190"/>
<attribute NAME="name" VALUE=": port name"/>
<attribute NAME="ptype" VALUE=": port type, e.g. sca_tdf::sca_out"/>
<attribute NAME="dtype" VALUE=": data type"/>
<attribute NAME="desc" VALUE=": description"/>
</node>
<node COLOR="#00b439" CREATED="1523881463972" HGAP="85" ID="ID_1043622050" MODIFIED="1523890539583" TEXT="module parameters : &lt;cpara&gt;s" VSHIFT="24">
<edge STYLE="bezier" WIDTH="thin"/>
<linktarget COLOR="#b0b0b0" DESTINATION="ID_1043622050" ENDARROW="Default" ENDINCLINATION="532;-64;" ID="Arrow_ID_898461556" SOURCE="ID_918524561" STARTARROW="None" STARTINCLINATION="290;0;"/>
<font NAME="SansSerif" SIZE="16"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="38" VALUE_WIDTH="108"/>
<attribute NAME="name" VALUE=": parameter name"/>
<attribute NAME="dtype" VALUE=": data type"/>
<attribute NAME="defval" VALUE=": default value"/>
<attribute NAME="desc" VALUE=": description"/>
</node>
<node COLOR="#00b439" CREATED="1523882206880" HGAP="70" ID="ID_1967515173" MODIFIED="1523890542609" VSHIFT="-149">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      User defeined functions : &lt;ufunc&gt;s
    </p>
  </body>
</html>
</richcontent>
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="53" VALUE_WIDTH="245"/>
<attribute NAME="name" VALUE=": function name"/>
<attribute NAME="accessor" VALUE=": accessor type, public, private, protected"/>
<attribute NAME="desc" VALUE=": description"/>
<node COLOR="#990000" CREATED="1523884608656" HGAP="69" ID="ID_1814370173" MODIFIED="1523890534401" TEXT="&lt;return&gt;" VSHIFT="44">
<font NAME="SansSerif" SIZE="14"/>
<attribute_layout NAME_WIDTH="33"/>
<attribute NAME="type" VALUE=": return type"/>
<attribute NAME="desc" VALUE=": description"/>
</node>
<node COLOR="#990000" CREATED="1523884655604" HGAP="59" ID="ID_480016871" MODIFIED="1523890532203" TEXT="function parameters &lt;para&gt;s" VSHIFT="44">
<edge STYLE="linear" WIDTH="1"/>
<font NAME="SansSerif" SIZE="14"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="38" VALUE_WIDTH="108"/>
<attribute NAME="name" VALUE=": parameter name"/>
<attribute NAME="dtype" VALUE=": data type"/>
<attribute NAME="defval" VALUE=": default value"/>
<attribute NAME="desc" VALUE=": description"/>
</node>
<node COLOR="#990000" CREATED="1523884754270" HGAP="72" ID="ID_1904777893" MODIFIED="1523890536696" TEXT="user defined code section &lt;code&gt;" VSHIFT="8">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
<node COLOR="#0033ff" CREATED="1523881277811" HGAP="52" ID="ID_1054077721" MODIFIED="1523890472059" POSITION="right" TEXT="&lt;info&gt;" VSHIFT="-64">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<attribute_layout NAME_WIDTH="73" VALUE_WIDTH="93"/>
<attribute NAME="name" VALUE=": project name"/>
<attribute NAME="author" VALUE=": project author"/>
<attribute NAME="date" VALUE=": created date"/>
<attribute NAME="desc" VALUE=":description"/>
<attribute NAME="toplevelOnly" VALUE=": true or false"/>
</node>
<node COLOR="#0033ff" CREATED="1523881282981" HGAP="13" ID="ID_202802311" MODIFIED="1523884859366" POSITION="right" TEXT="&lt;toplevel&gt;" VSHIFT="-92">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1523881509164" HGAP="119" ID="ID_1739219555" MODIFIED="1523890444677" TEXT="&lt;sim_conf&gt;" VSHIFT="-13">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<attribute_layout NAME_WIDTH="49" VALUE_WIDTH="169"/>
<attribute NAME="tstep" VALUE=":time step in second"/>
<attribute NAME="tsim" VALUE=": simulation duration"/>
<attribute NAME="tracefile" VALUE=": trace file format, vcd or csv"/>
</node>
<node COLOR="#00b439" CREATED="1523881511009" HGAP="124" ID="ID_23097189" MODIFIED="1523890447404" VSHIFT="-16">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &lt;instance&gt;s
    </p>
  </body>
</html>
</richcontent>
<edge STYLE="bezier" WIDTH="thin"/>
<arrowlink DESTINATION="ID_188966571" ENDARROW="Default" ENDINCLINATION="-36;79;" ID="Arrow_ID_1041239702" STARTARROW="None" STARTINCLINATION="292;0;"/>
<font NAME="SansSerif" SIZE="16"/>
<icon BUILTIN="list"/>
<attribute_layout NAME_WIDTH="46" VALUE_WIDTH="137"/>
<attribute NAME="name" VALUE=": instance name"/>
<attribute NAME="module" VALUE=": refered module name"/>
<node COLOR="#990000" CREATED="1523883368091" HGAP="108" ID="ID_918524561" LINK="#ID_1043622050" MODIFIED="1523890409207" VSHIFT="4">
<richcontent TYPE="NODE"><html>
  <head>
    
  </head>
  <body>
    <p>
      instance parameter &lt;para&gt;s
    </p>
  </body>
</html>
</richcontent>
<arrowlink DESTINATION="ID_1043622050" ENDARROW="Default" ENDINCLINATION="532;-64;" ID="Arrow_ID_898461556" STARTARROW="None" STARTINCLINATION="290;0;"/>
<font NAME="SansSerif" SIZE="14"/>
<attribute_layout NAME_WIDTH="35" VALUE_WIDTH="155"/>
<attribute NAME="name" VALUE=": refered parameter name"/>
<attribute NAME="val" VALUE=": parameter value"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1523881511985" HGAP="127" ID="ID_394384186" MODIFIED="1523890456797" TEXT="&lt;interconnect&gt;" VSHIFT="-59">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
</node>
</map>
