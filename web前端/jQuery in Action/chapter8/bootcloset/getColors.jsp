[
    {value:'',caption:'choose color'},
<c:choose>
  <c:when test="${param.style=='7141832'}">
    {value:'bk',caption:'Black Oil-tanned'},
    {value:'br',caption:'Black Polishable'}
  </c:when>
  <c:when test="${param.style=='7173656'}">
    {value:'bk',caption:'Black'},
    {value:'br',caption:'Crazy Horse'}
  </c:when>
  <c:when test="${param.style=='7141922'}">
    {value:'bk',caption:'Black'},
    {value:'br',caption:'Crazy Horse'}
  </c:when>
  <c:when test="${param.style=='7177382'}">
    {value:'hy',caption:'Honey'},
    {value:'sd',caption:'Peanut'}
  </c:when>
  <c:when test="${param.style=='7141730'}">
    {value:'rw',caption:'Brown'},
    {value:'hy',caption:'Honey'}
  </c:when>
  <c:when test="${param.style=='7141833'}">
    {value:'br',caption:'Russet'}
  </c:when>
  <c:when test="${param.style=='7257914'}">
    {value:'br',caption:'Brown'}
  </c:when>
  <c:when test="${param.style=='7269643'}">
    {value:'bk',caption:'Black'}
  </c:when>
  <c:otherwise>
    {value:'bk',caption:'Black'}
  </c:otherwise>
</c:choose>
]
