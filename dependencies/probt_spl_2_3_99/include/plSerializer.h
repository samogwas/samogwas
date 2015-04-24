/*=============================================================================
 * Product        : ProBT 
 * File           : plSerializer.h
 * Author         : David Raulo
 * Creation       : Tue Jul 24 17:05:09 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

/// \file plSerializer.h
/// Serialization for ProBT objects.
/// \authors David Raulo, Juan-Manuel Ahuactzin

#ifndef PL_SERIALIZER_H
#define PL_SERIALIZER_H

#include <plConfig.h>
#include <pl.h>
#include <plDebug.h>

//-----------------------------------------------------------------------------
/** The purpose of this class is to register ProBT objects in order to save
    them to file archives and later restoring them.
    @em Warning: the file format compatibility is only guaranteed with versions
    of ProBT sharing the same major and minor numbers than the version used to
    save the archive. \sa plVersion::get_version()
*/
class PL_DLL_API plSerializer
{
public:

  /// Archive type, passed as argument to load() and save().
  enum ArchiveType { BINARY_ARCHIVE, XML_ARCHIVE };

  typedef std::map< std::string, plObject* > ObjectsMap;

  /// Constructor
  plSerializer(const std::string &name = "untitled");

  /// Destructor
  virtual ~plSerializer();

  /// Set the name.
  void set_name(const std::string &name) { name_ = name; }

  /// Get the program name.
  const std::string &get_name() const { return name_; }

  /// Load from an XML or binary file.
  /// If unspecified, the default format is XML.
  void load(const std::string &fileName, 
            plSerializer::ArchiveType format = XML_ARCHIVE);
  
  /// Load from an XML or binary input stream.
  /// If unspecified, the default format is XML.
  void load(std::istream& in,
            plSerializer::ArchiveType format = XML_ARCHIVE);

  /// Save all contained objects to an XML or binary file.
  /// If unspecified, the default format is XML.
  void save(const std::string &fileName,
            plSerializer::ArchiveType format = XML_ARCHIVE) const;

  /// Save all contained objects to an XML or binary output stream.
  /// If unspecified, the default format is XML.
  void save(std::ostream& out,
            plSerializer::ArchiveType format = XML_ARCHIVE) const;

  /// Delete all contained objets.
  virtual void clear();

  /** Register an object.
      If another object was previously registered with the same name, the new
      one will replace it without any warning.
  */
  template <class T>
    void add_object(const std::string &name_obj, const T &obj);

  /** Unregister an object.
      If no object has been previously registered under this name, the
      function returns false. It returns true otherwise.
  */
  bool remove_object(const std::string &name_obj);

  /** Returns @em true is object with name @em name_obj already exists.
      Returns @em false otherwise.
  */
  bool object_exists(const std::string &name_obj)const;
    
  /** Get object by name.
      \return @em false if no object of the relevant type is contained.
  */
  template <class T> bool get_object(const std::string &name_obj, T &obj);

  /// Get a map of all objects of a given type T.
  template <class T>
    void get_objects_map(std::map<std::string, T*>& object_map);

  /// Get a map of all objects of a given type T.
  template <class T>
    void get_objects_map(std::map<std::string, const T*> & object_map) const;

  /// Get all the registered objects.
  const std::map< std::string, plObject* >
    &get_all_objects_map() const { return objects_; }

  /// Print the list of contained object of a certain type T.
  template <class T> 
    void print_object_class_list(std::ostream& out, T* obj, 
                                 const std::string &class_name) const;

  /// Print all contained objects to an output stream.
  void print(std::ostream& out) const;

  //---------------------------------------------------------------------------
  /** Check if the serializer class is in strict mode.
      \sa set_strict_mode()
  */
  static bool get_strict_mode() { return strict_mode_; }

  /** Choose if the serializer class should operate in strict mode.

      Strict mode means that ProBT will refuse to load a serialization
      archive if either the ProBT version, the BOOST version or the compiler
      of the archive creator was different from those of the running ProBT.

      The default is false, which means that ProBT will attempt to load
      any archive, possibly leading to a crash.
  */
  static void set_strict_mode(bool m) { strict_mode_ = m; }

  /** Check whether a file is compatible with the running version of ProBT. */
  static
  bool check_s11n_version_compatibility(const std::string& s11n_file_version);
  
  /** Get the S11N format version for a given ProBT release. */
  static
  unsigned int get_s11n_compatibility_version(const plVersion& probt_version);
  
  //---------------------------------------------------------------------------
protected:
  template <class archive>
    void load_objects(archive& ia);
  template <class archive>
    void save_objects(archive& oa) const;

  std::string name_;
  ObjectsMap objects_;
  static bool strict_mode_;
};

//-----------------------------------------------------------------------------
template <class T>
bool plSerializer::get_object(const std::string &name_obj, T &obj)
{
  ObjectsMap::iterator obj_ptr;
        
  obj_ptr = objects_.find(name_obj);
        
  if (obj_ptr != objects_.end())
  {
    T* specialized = dynamic_cast<T*>(obj_ptr->second);
    if (specialized)
    {
      obj = *specialized; 
      return true;
    }
    else return false;
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------
template <class T>
void plSerializer::get_objects_map(std::map<std::string, T*> & object_map)
{
  ObjectsMap::const_iterator obj_ptr;

  object_map.clear();

  for (obj_ptr = objects_.begin(); obj_ptr != objects_.end(); ++obj_ptr)
  {
    T* specialized = dynamic_cast<T*>(obj_ptr->second);
    if (specialized)
    {
      object_map[ obj_ptr->first ] = specialized;
    }
  }
}

//-----------------------------------------------------------------------------
template <class T>
void plSerializer::get_objects_map(std::map<std::string,
                                   const T*> & object_map) const
{
  ObjectsMap::const_iterator obj_ptr;

  object_map.clear();

  for (obj_ptr = objects_.begin(); obj_ptr != objects_.end(); ++obj_ptr)
  {
    T* specialized = dynamic_cast<T*>(obj_ptr->second);
    if (specialized)
    {
      object_map[ obj_ptr->first ] = specialized;
    }
  }
}

//-----------------------------------------------------------------------------
template <class T>
void plSerializer::print_object_class_list(std::ostream& out, T* /* obj */, 
                                           const std::string &class_name) const
{
  ObjectsMap::const_iterator obj_ptr;
  unsigned int count;
        
  out << "---- " << class_name << " ----" << std::endl;
  count = 0;
  for (obj_ptr = objects_.begin(); obj_ptr != objects_.end(); ++obj_ptr)
  {
    if (dynamic_cast<T*>(obj_ptr->second))
    {
      out << obj_ptr->first << ": ";
      pl_print_object_info(out, obj_ptr->second);
      count++;
    }
  }
  out << count <<" " << class_name << " registered." << std::endl << std::endl;
}

//-----------------------------------------------------------------------------
template <class T> 
void plSerializer::add_object(const std::string &name_obj, const T &obj)
{
  ObjectsMap::iterator obj_ptr = objects_.find(name_obj);
    
  if(obj_ptr != objects_.end()){
    delete obj_ptr->second;
  }
    
  objects_[name_obj] = new T(obj);
}

//-----------------------------------------------------------------------------
#endif //PL_SERIALIZER_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
