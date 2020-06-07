void Euler( cpl::Vector& x, double& tau, cpl::Vector derivs(const cpl::Vector&))
 {
      x += tau * derivs(x);
 }