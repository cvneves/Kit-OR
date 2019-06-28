void solve(Data &data)
{
   IloEnv env;
   IloModel model(env);

   IloNumVarArray lambda(env, data.getNItems(), 0.0, 1.0);

   {
      IloExpr sum(env);
      for (int i = 0; i < data.getNItems(); i++)
      {
         sum += lambda[i];
         model.add(lambda[i] == 1);
      }

      model.add(IloMinimize(env, sum));
      sum.end();
   }

   IloCplex BPP(model);

   try
   {
      BPP.solve();
   }

   catch (IloException &e)
   {
      std::cout << e;
   }

   std::cout << "Status: " << BPP.getStatus() << "\n";
   std::cout << "Bins usados: " << BPP.getObjValue() << "\n";

   

   env.end();
}
