<?xml version="1.0" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="page">
    <html><head>
	<xsl:apply-templates select="meta" />
    </head>
    <body bgcolor="black">
	    <xsl:apply-templates />    
    </body>
    </html>
</xsl:template>

<xsl:template match="xtable">
    <table align="center">
	<xsl:apply-templates />
    </table>
</xsl:template>

<xsl:template match="xtr">
    <tr>
	<xsl:apply-templates />
    </tr>
</xsl:template>

<xsl:template match="xtd">
    <td>
	<xsl:apply-templates />
    </td>
</xsl:template>

<xsl:template match="xp">
    <p>
	<xsl:apply-templates />
    </p>
</xsl:template>

</xsl:stylesheet>