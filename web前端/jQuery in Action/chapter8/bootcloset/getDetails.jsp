<jsp:useBean id="item" class="java.util.HashMap"/>
<c:choose>
  <c:when test="${param.style=='7141832'}">
      <c:set target="${item}" property="name" value="Chippewa 17-inch Engineer Boot"/>
      <c:set target="${item}" property="sku" value="7141832"/>
      <c:set target="${item}" property="height" value="17 inches"/>
      <c:set target="${item}" property="lining" value="Leather"/>
      <c:set target="${item}" property="colors" value="Black <abbr>Oil-tanned</abbr>, Black Polishable"/>
      <c:set target="${item}" property="price" value="$187.00"/>
      <c:set target="${item}" property="features" value="<abbr>Oil-tanned</abbr> or polishable <abbr>full-grain</abbr> leather uppers. <abbr>Vibram</abbr> sole. <abbr>Goodyear welt</abbr>. Padded insole. Steel safety toe."/>
  </c:when>
  <c:when test="${param.style=='7173656'}">
      <c:set target="${item}" property="name" value="Chippewa 11-inch Engineer Boot"/>
      <c:set target="${item}" property="sku" value="7173656"/>
      <c:set target="${item}" property="height" value="11 inches"/>
      <c:set target="${item}" property="lining" value="None"/>
      <c:set target="${item}" property="colors" value="Black, Crazy Horse"/>
      <c:set target="${item}" property="price" value="$167.00"/>
      <c:set target="${item}" property="features" value="<abbr>Oil-tanned</abbr> <abbr>full-grain</abbr> leather uppers. <abbr>Vibram</abbr> sole. <abbr>Goodyear welt</abbr>. Padded insole. Steel safety toe. Texon&reg; insole."/>
  </c:when>
  <c:when test="${param.style=='7141922'}">
      <c:set target="${item}" property="name" value="Chippewa Harness Boot"/>
      <c:set target="${item}" property="sku" value="7141922"/>
      <c:set target="${item}" property="height" value="13 inches"/>
      <c:set target="${item}" property="lining" value="Leather"/>
      <c:set target="${item}" property="colors" value="Black, Crazy Horse"/>
      <c:set target="${item}" property="price" value="$187.00"/>
      <c:set target="${item}" property="features" value="<abbr>Full-grain</abbr> leather uppers. Leather lining. <abbr>Vibram</abbr> sole. <abbr>Goodyear welt</abbr>."/>
  </c:when>
  <c:when test="${param.style=='7177382'}">
      <c:set target="${item}" property="name" value="Caterpillar Tradesman Work Boot"/>
      <c:set target="${item}" property="sku" value="7177382"/>
      <c:set target="${item}" property="height" value="6 inches"/>
      <c:set target="${item}" property="lining" value="Leather"/>
      <c:set target="${item}" property="colors" value="Honey, Peanut"/>
      <c:set target="${item}" property="price" value="$87.00"/>
      <c:set target="${item}" property="features" value="<abbr>Full-grain</abbr> <abbr>oil-tanned</abbr> leather. Nylon mesh lining. Ortholite sock liner. EVA midsole. T84V Rubberlon outsole."/>
  </c:when>
  <c:when test="${param.style=='7141730'}">
      <c:set target="${item}" property="name" value="Danner Foreman Pro Work Boot"/>
      <c:set target="${item}" property="sku" value="7141730"/>
      <c:set target="${item}" property="height" value="10 inches"/>
      <c:set target="${item}" property="lining" value="Leather"/>
      <c:set target="${item}" property="colors" value="Honey, Brown"/>
      <c:set target="${item}" property="price" value="$287.00"/>
      <c:set target="${item}" property="features" value="Alkali-resistant <abbr>full-grain</abbr> leather. <abbr>Cambrelle</abbr> nylon lining. Fiberglass shank. <abbr>Vibram</abbr> 4014 Cristy sole. <abbr>Stitch-down</abbr> construction."/>
  </c:when>
  <c:when test="${param.style=='7141833'}">
      <c:set target="${item}" property="name" value="Chippewa 17-inch Snakeproof Boot"/>
      <c:set target="${item}" property="sku" value="7141833"/>
      <c:set target="${item}" property="height" value="17 inches"/>
      <c:set target="${item}" property="lining" value="Leather"/>
      <c:set target="${item}" property="colors" value="Russet"/>
      <c:set target="${item}" property="price" value="$147.00"/>
      <c:set target="${item}" property="features" value="<abbr>Full-grain</abbr> leather foot. 1000 Denier <abbr>Cordura</abbr> Viper cloth shaft. <abbr>Goodyear welt</abbr>. Leather Lining. Body Cushion Insole. <abbr>Vibram</abbr> Robinson Outsole."/>
  </c:when>
  <c:when test="${param.style=='7257914'}">
      <c:set target="${item}" property="name" value="Danner Grouse GTX Boot"/>
      <c:set target="${item}" property="sku" value="7257914"/>
      <c:set target="${item}" property="height" value="8 inches"/>
      <c:set target="${item}" property="lining" value="<abbr>Gore-Tex</abbr>"/>
      <c:set target="${item}" property="colors" value="Brown"/>
      <c:set target="${item}" property="price" value="$207.00"/>
      <c:set target="${item}" property="features" value="<abbr>Full-grain</abbr> leather foot. 1000 Denier <abbr>Cordura</abbr> Viper cloth shaft. <abbr>Gore-Tex</abbr> lining. Stich-down construction."/>
  </c:when>
  <c:when test="${param.style=='7269643'}">
      <c:set target="${item}" property="name" value="Caterpillar Logger Boot"/>
      <c:set target="${item}" property="sku" value="7269643"/>
      <c:set target="${item}" property="height" value="8 inches"/>
      <c:set target="${item}" property="lining" value="<abbr>Cambrelle</abbr>"/>
      <c:set target="${item}" property="colors" value="Black"/>
      <c:set target="${item}" property="price" value="$157.00"/>
      <c:set target="${item}" property="features" value="<abbr>Full-grain</abbr> leather. <abbr>Cambrelle</abbr>&reg; lining. Steel safety toe. Electrical hazard protection. Poliyou&reg; cushion insole. Rubber lug outsole."/>
  </c:when>
  <c:otherwise>
      <c:set target="${item}" property="name" value="---"/>
      <c:set target="${item}" property="sku" value="---"/>
      <c:set target="${item}" property="height" value="---"/>
      <c:set target="${item}" property="lining" value="---"/>
      <c:set target="${item}" property="colors" value="---"/>
      <c:set target="${item}" property="price" value="---"/>
      <c:set target="${item}" property="features" value="---"/>
  </c:otherwise>
</c:choose>
<div>
  <label>Item name:</label> ${item.name}
</div>
<div>
  <label>SKU:</label> ${item.sku}
</div>
<div>
  <label>Height:</label> ${item.height}
</div>
<div>
  <label>Colors:</label> ${item.colors}
</div>
<div>
  <label>Lining:</label> ${item.lining}
</div>
<div>
  <label>Today's price:</label> ${item.price}
</div>
<div>
  <label>Features:</label> ${item.features}
</div>
<div align="center">
  <c:if test="${item.sku != '---'}">
    <img id="itemPhoto" src="photos/${item.sku}.png"/>
  </c:if>
</div>
