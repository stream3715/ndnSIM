// major
  FibHelper::AddRoute(Names::Find<Node>("rtr-2"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-1"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-1"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-3"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-3"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-34"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-34"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-" + vecProdId[0]), 0);
  
  FibHelper::AddRoute(Names::Find<Node>("rtr-4"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-5"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-5"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-3"), 0);
  
  FibHelper::AddRoute(Names::Find<Node>("rtr-13"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-11"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-11"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-20"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-14"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-27"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-27"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-5"), 0);
  
  FibHelper::AddRoute(Names::Find<Node>("rtr-19"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-25"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-25"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-26"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-26"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-27"), 0);

  FibHelper::AddRoute(Names::Find<Node>("rtr-16"), ndn::Name(prefix + "/major"), Names::Find<Node>("rtr-5"), 0);
  
  // minor
  FibHelper::AddRoute(Names::Find<Node>("rtr-3"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-5"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-5"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-27"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-27"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-26"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-26"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-25"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-25"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-" + vecProdId[1]), 0);

  FibHelper::AddRoute(Names::Find<Node>("rtr-14"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-11"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-11"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-20"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-20"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-25"), 0);

  FibHelper::AddRoute(Names::Find<Node>("rtr-15"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-5"), 0);
  
  FibHelper::AddRoute(Names::Find<Node>("rtr-9"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-24"), 0);
  FibHelper::AddRoute(Names::Find<Node>("rtr-24"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-20"), 0);

  FibHelper::AddRoute(Names::Find<Node>("rtr-18"), ndn::Name(prefix + "/minor"), Names::Find<Node>("rtr-11"), 0);