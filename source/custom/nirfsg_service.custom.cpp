#include <nirfsg/nirfsg_service.h>
using nidevice_grpc::converters::convert_to_grpc;
using nidevice_grpc::converters::convert_from_grpc;
namespace nirfsg_grpc {

  // Returns true if it's safe to use outputs of a method with the given status.
  inline bool status_ok(int32 status)
  {
    return status >= 0;
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiRFSGService::ConvertApiErrorStatusForViSession(::grpc::ServerContextBase* context, int32_t status, ViSession vi)
  {
    static_assert(nidevice_grpc::kMaxGrpcErrorDescriptionSize >= 1024, "ErrorMessage expects a minimum buffer size.");
    ViStatus error_code{};
    std::string description(nidevice_grpc::kMaxGrpcErrorDescriptionSize, '\0');
    // Try first to get the most recent error with a dynamic message.
    library_->GetError(vi, &error_code, nidevice_grpc::kMaxGrpcErrorDescriptionSize, &description[0]);
    if (error_code != status) {
      // Since another thread has changed the status, fall back to the static message lookup.
      description.assign(nidevice_grpc::kMaxGrpcErrorDescriptionSize, '\0');
      library_->ErrorMessage(vi, status, &description[0]);
    }
    return nidevice_grpc::ApiErrorAndDescriptionToStatus(context, status, description);
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiRFSGService::WriteArbWaveformComplexF32(::grpc::ServerContext* context, const WriteArbWaveformComplexF32Request* request, WriteArbWaveformComplexF32Response* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto vi_grpc_session = request->vi();
      ViSession vi = session_repository_->access_session(vi_grpc_session.name());
      auto waveform_name_mbcs = convert_from_grpc<std::string>(request->waveform_name());
      auto waveform_name = waveform_name_mbcs.c_str();

      // Get pointer to interleaved float array
      const ViReal32* wfm_data = request->wfm_data().data();
      // Reinterpret as array of NIComplexNumberF32_struct
      const NIComplexNumberF32_struct* wfm_data_ptr = reinterpret_cast<const NIComplexNumberF32_struct*>(wfm_data);
      // Number of samples is half the float count
      ViInt32 number_of_samples = static_cast<ViInt32>(request->wfm_data().size() / 2);

      ViBoolean more_data_pending = request->more_data_pending();
      auto status = library_->WriteArbWaveformComplexF32(vi, waveform_name, number_of_samples, wfm_data_ptr, more_data_pending);
      if (!status_ok(status)) {
        return ConvertApiErrorStatusForViSession(context, status, vi);
      }
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::NonDriverException& ex) {
      return ex.GetStatus();
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiRFSGService::WriteArbWaveformComplexF64(::grpc::ServerContext* context, const WriteArbWaveformComplexF64Request* request, WriteArbWaveformComplexF64Response* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto vi_grpc_session = request->vi();
      ViSession vi = session_repository_->access_session(vi_grpc_session.name());
      auto waveform_name_mbcs = convert_from_grpc<std::string>(request->waveform_name());
      auto waveform_name = waveform_name_mbcs.c_str();

      // Get pointer to interleaved double array
      const ViReal64* wfm_data = request->wfm_data().data();
      // Reinterpret as array of NIComplexNumber_struct
      const NIComplexNumber_struct* wfm_data_ptr = reinterpret_cast<const NIComplexNumber_struct*>(wfm_data);
      // Number of samples is half the double count
      ViInt32 number_of_samples = static_cast<ViInt32>(request->wfm_data().size() / 2);

      ViBoolean more_data_pending = request->more_data_pending();
      auto status = library_->WriteArbWaveformComplexF64(vi, waveform_name, number_of_samples, wfm_data_ptr, more_data_pending);
      if (!status_ok(status)) {
        return ConvertApiErrorStatusForViSession(context, status, vi);
      }
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::NonDriverException& ex) {
      return ex.GetStatus();
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiRFSGService::WriteArbWaveformComplexI16(::grpc::ServerContext* context, const WriteArbWaveformComplexI16Request* request, WriteArbWaveformComplexI16Response* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto vi_grpc_session = request->vi();
      ViSession vi = session_repository_->access_session(vi_grpc_session.name());
      auto waveform_name_mbcs = convert_from_grpc<std::string>(request->waveform_name());
      auto waveform_name = waveform_name_mbcs.c_str();

      // Get pointer to interleaved int16 array
      const ViInt16* wfm_data = request->wfm_data().data();
      // Reinterpret as array of NIComplexI16_struct
      const NIComplexI16_struct* wfm_data_ptr = reinterpret_cast<const NIComplexI16_struct*>(wfm_data);
      // Number of samples is half the int16 count
      ViInt32 number_of_samples = static_cast<ViInt32>(request->wfm_data().size() / 2);

      auto status = library_->WriteArbWaveformComplexI16(vi, waveform_name, number_of_samples, wfm_data_ptr);
      if (!status_ok(status)) {
        return ConvertApiErrorStatusForViSession(context, status, vi);
      }
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::NonDriverException& ex) {
      return ex.GetStatus();
    }
  }
}  // namespace nirfsg_grpc
