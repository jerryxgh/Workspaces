<c:forEach items="${pageContext.request.parameterMap}" var="item">
  <div><b>${item.key}</b>=<c:forEach items="${item.value}" var="value" varStatus="loop">${loop.first ? '':','}${value}</c:forEach></div>
</c:forEach>
<%
  System.out.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  System.out.println(request.getContentType());
%>